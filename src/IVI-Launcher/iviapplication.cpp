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
#include <iviapplicationmanager_p.h>
#include <ivisingletonmanager_p.h>
#include <iviappprocessmanager_p.h>

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
    IVIApplicationManagerPrivate::registerApplication(this);
}

const QList<QWaylandIviSurface*>& IVIApplication::surfaces() const {
    Q_D(const IVIApplication);
    return d->surfaces;
}

const QString& IVIApplication::getName() const {
    Q_D(const IVIApplication);
    return d->name;
}

const QString& IVIApplication::getDescription() const {
    Q_D(const IVIApplication);
    return d->description;
}

const QString& IVIApplication::getIcon() const {
    Q_D(const IVIApplication);
    return d->icon;
}

const QString& IVIApplication::getWebsite() const {
    Q_D(const IVIApplication);
    return d->website;
}

const QList<QString>& IVIApplication::getCategories() const {
    Q_D(const IVIApplication);
    return d->categories;
}

const QString& IVIApplication::getExecutable() const {
    Q_D(const IVIApplication);
    return d->executable;
}

const QList<QString>& IVIApplication::getArguments() const {
    Q_D(const IVIApplication);
    return d->arguments;
}

const QString& IVIApplication::getWorkingDirectory() const {
    Q_D(const IVIApplication);
    return d->workingDirectory;
}

IVIApplication::RunningState IVIApplication::getRunningState() const {
    Q_D(const IVIApplication);
    return d->runningState;
}

IVIApplicationPrivate::IVIApplicationPrivate(const IVIManifest& manifest, IVIApplication*)
    : runningState(IVIApplication::RunningState::NotRunning)
    , name(manifest.getName())
    , description(manifest.getDescription())
    , icon(manifest.getIcon())
    , website(manifest.getWebsite())
    , categories(manifest.getCategories())
    , executable(manifest.getExecutable())
    , arguments(manifest.getArguments())
    , workingDirectory(QFileInfo(manifest.getFilename()).dir().absolutePath())
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

void IVIApplication::launch() {
    Q_D(IVIApplication);
    if(d->runningState != IVIApplication::RunningState::NotRunning)
        return;
    d->runningState = IVIApplication::RunningState::Starting;
    emit runningStateChanged();
    IVIAppProcessManager::getInstance().launch(*this);
}

void IVIApplicationPrivate::addSurface(QWaylandIviSurface* iviSurface) {
    Q_Q(IVIApplication);
    runningState = IVIApplication::RunningState::Running;
    emit q->runningStateChanged();
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
