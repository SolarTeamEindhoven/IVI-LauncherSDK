#include "iviapplicationmanager_p.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QTimer>

#include <ivisingletonmanager_p.h>
#include <IVIApplication>
#include <IVIManifest>

QT_BEGIN_NAMESPACE

namespace {
    static QString defaultApplicationsDirectoryName("/usr/share/ivi/apps");
}

IVIApplicationManager::IVIApplicationManager(QObject* parent)
    : QObject(*new IVIApplicationManagerPrivate(), parent)
{
    Q_D(IVIApplicationManager);
    d->initialize();
}

IVIApplicationManager::~IVIApplicationManager()
{}

QList<IVIApplication*> IVIApplicationManager::getApplicationList() const {
    Q_D(const IVIApplicationManager);
    return d->applications;
}

IVIApplicationManager& IVIApplicationManager::getInstance() {
    static IVIApplicationManager applicationManager;
    return applicationManager;
}

IVIApplicationManagerPrivate::IVIApplicationManagerPrivate()
{}

IVIApplicationManagerPrivate::~IVIApplicationManagerPrivate() {
    foreach(IVIApplication* application, applications)
        application->deleteLater();
}

void IVIApplicationManagerPrivate::initialize() {
    Q_Q(IVIApplicationManager);

    QString directoryName = defaultApplicationsDirectoryName;

    if(qEnvironmentVariableIsSet("IVI_APP_PATH"))
        directoryName = QString::fromLatin1(qgetenv("IVI_APP_PATH"));

    QDir directory(directoryName);
    directory.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QDirIterator it(directory, QDirIterator::FollowSymlinks);
    bool applicationsChanged = false;
    while(it.hasNext())
    {
        QDir dir(it.next());
        if(!dir.exists(QStringLiteral("MANIFEST")))
            continue;

        IVIManifest manifest(dir.filePath(QStringLiteral("MANIFEST")));
        if(!manifest.isSuccesful())
            continue;

        IVIApplication* application = new IVIApplication(manifest);
        applications.append(application);
        applicationsChanged = true;
    }

    if(applicationsChanged)
        QTimer::singleShot(0, q, &IVIApplicationManager::applicationListChanged);
}

void IVIApplicationManagerPrivate::registerApplication(IVIApplication* application) {
    IVIApplicationManager::getInstance().d_func()->handleApplicationRegistration(application);
}

const QList<IVIApplication*>& IVIApplicationManagerPrivate::getApplications() noexcept {
    return IVIApplicationManager::getInstance().d_func()->applications;
}

void IVIApplicationManagerPrivate::handleApplicationRegistration(IVIApplication* application) {
    Q_Q(IVIApplicationManager);
    applications.append(application);
    emit q->applicationListChanged();
}

QT_END_NAMESPACE
