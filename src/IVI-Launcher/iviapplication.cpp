#include "iviapplication.h"
#include "iviapplication_p.h"

#include <mutex>
#include <unordered_map>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtWaylandCompositor/QWaylandClient>
#include <QtWaylandCompositor/QWaylandSurface>
#include <QtWaylandCompositor/QWaylandIviSurface>

#include <IVIManifest>
#include <IVIApplicationManager>

QT_BEGIN_NAMESPACE

namespace {
static std::mutex IVIApplicationMapMutex;
static std::unordered_map<qint64, IVIApplication*> IVIApplicationMap;
static QHash<QString, IVIApplication*> manifestFiles;
}

IVIApplication::IVIApplication(const IVIManifest& manifest, QObject* parent)
    : IVIApplication(*new IVIApplicationPrivate(manifest, this), parent)
{}

IVIApplication::IVIApplication(IVIApplicationPrivate& dd, QObject* parent)
    : QObject(dd, parent)
{
    IVIApplicationManager::registerApplication(this);
}

const QList<QWaylandIviSurface*>& IVIApplication::surfaces() const {
    Q_D(const IVIApplication);
    return d->surfaces;
}

const QString& IVIApplication::getName() const {
    Q_D(const IVIApplication);
    return d->name;
}

IVIApplicationPrivate::IVIApplicationPrivate(const IVIManifest& manifest, IVIApplication*)
    : name(manifest.getName())
    , description(manifest.getDescription())
    , icon(manifest.getIcon())
    , website(manifest.getWebsite())
    , categories(manifest.getCategories())
    , executable(manifest.getExecutable())
    , arguments(manifest.getArguments())
{}

static QString executingDirFromPID(qint64 PID)
{
    QString exe = QFile::readLink("/proc/" + QString::number(PID) + "/exe");
    return QFileInfo(exe).canonicalPath();
}

IVIApplication* IVIApplication::fromWaylandClient(QWaylandClient* client) {
    static bool debugMode = qEnvironmentVariableIsSet("STE_ENABLE_DEBUG");

    auto processId = client->processId();

    std::unique_lock<std::mutex> lock(IVIApplicationMapMutex);
    auto it = IVIApplicationMap.find(processId);

    if(it == IVIApplicationMap.end()) {
        if(!debugMode)
            return nullptr;

        QDir dir(executingDirFromPID(processId));
        if(!dir.exists(QStringLiteral("MANIFEST")))
            return nullptr;

        auto manifestFile = dir.filePath(QStringLiteral("MANIFEST"));
        const auto it2 = manifestFiles.find(manifestFile);
        if(it2 != manifestFiles.end())
            return it2.value();

        IVIManifest manifest(manifestFile);
        if(!manifest.isSuccesful())
            return nullptr;

        IVIApplication* application = new IVIApplication(manifest);
        IVIApplicationMap.insert({processId, application});
        manifestFiles.insert(manifestFile, application);
        return application;
    }

    return it->second;
}

void IVIApplicationPrivate::addSurface(QWaylandIviSurface* iviSurface) {
    Q_Q(IVIApplication);
    connect(iviSurface, &QWaylandIviSurface::destroyed, this, &IVIApplicationPrivate::handleIviSurfaceDestroy);
    surfaces.append(iviSurface);
    emit q->newSurface(iviSurface);
}

void IVIApplicationPrivate::handleIviSurfaceDestroy(QObject* obj) {
    if(obj != nullptr) {
        auto ptr = static_cast<QWaylandIviSurface*>(obj);
        surfaces.removeOne(ptr);
    }
}

QT_END_NAMESPACE
