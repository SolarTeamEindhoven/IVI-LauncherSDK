#include "steappinstance.h"

#include <QList>
#include <QDir>
#include <QDebug>

#include "steapp.h"
#include "steappmanager.h"
#include "wayland/steshellsurface_wl_p.h"

static QList<STEAppInstance*> STEAppInstanceList;

STEAppInstance::STEAppInstance(STEApp* app, QObject* parent)
    : QObject(parent)
    , app(app)
{
    STEAppInstanceList.append(this);

    if(app)
        app->addAppInstance(this);

    qDebug() << "Create app instance at:" << static_cast<void*>(this);
}

STEAppInstance::~STEAppInstance()
{
    STEAppInstanceList.removeOne(this);

    if(app)
        app->removeAppInstance(this);

    qDebug() << "Deleted app instance at:" << static_cast<void*>(this);
}

STEApp* STEAppInstance::getApp() const
{
    return app;
}

quint64 STEAppInstance::getPID() const
{
    if(shellsurfaces.count())
        return shellsurfaces[0]->getSurface()->client()->processId();

    return 0;
}

static QString executingDirFromPID(qint64 PID)
{
    QString exe = QFile::readLink("/proc/" + QString::number(PID) + "/exe");
    return QFileInfo(exe).canonicalPath();
}

STEAppInstance* STEAppInstance::fromPID(qint64 PID)
{
    foreach(STEAppInstance* instance, STEAppInstanceList)
    {
        if(instance->getPID() == PID)
        {
            return instance;
        }
    }

    if(qEnvironmentVariableIsSet("STE_ENABLE_DEBUG"))
    {
        qDebug() << "Attempting to create new app instance for process ID:" << PID;

        QDir dir(executingDirFromPID(PID));

        STEApp* app = nullptr;
        foreach(STEApp* appEntry, STEAppManager::getAppList())
        {
            if(dir.canonicalPath() == QDir(appEntry->getWorkingDirectory()).canonicalPath())
            {
                app = appEntry;
                break;
            }
        }

        if(app == nullptr && dir.exists("MANIFEST"))
        {
            qDebug() << "Creating new app!";
            app = new STEApp(dir);
        }

        if(app == nullptr)
        {
            qDebug() << "Could not find proper MANIFEST file unregistered applicaiton. STEShellSurface will NOT be displayed!";
        }
        else
        {
            qDebug() << "Creating new app instance!";
            return new STEAppInstance(app);
        }
    }

    return nullptr;
}

void STEAppInstance::removeShellSurface()
{
    STEShellSurface_wl* shellSurface = qobject_cast<STEShellSurface_wl*>(sender());

    if(shellSurface == NULL)
        return;

    shellsurfaces.removeOne(shellSurface);

    if(shellsurfaces.isEmpty())
        deleteLater();
}

void STEAppInstance::addShellSurface(STEShellSurface_wl* newShellsurface)
{
    qDebug() << "Attemting to add shell surface to instance";

    foreach(STEShellSurface_wl* shellSurface, shellsurfaces)
    {
        if(shellSurface == newShellsurface)
            return;
    }

    connect(newShellsurface, &STEShellSurface_wl::surfaceDestroyed, this, &STEAppInstance::removeShellSurface);
    shellsurfaces.append(newShellsurface);

    qDebug() << "emit shellSurfaceAdded from" << static_cast<void*>(this);
    emit shellSurfaceAdded(newShellsurface);
}
