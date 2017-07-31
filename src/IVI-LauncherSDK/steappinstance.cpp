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
}

STEAppInstance::~STEAppInstance()
{
    STEAppInstanceList.removeOne(this);

    if(app)
        app->removeAppInstance(this);
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
            app = new STEApp(dir);
        }

        if(app == nullptr)
        {
            qDebug() << "Could not find proper MANIFEST file unregistered applicaiton. STEShellSurface will NOT be displayed!";
        }
        else
        {
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

    disconnect(shellSurface, &STEShellSurface_wl::surfaceDestroyed, this, &STEAppInstance::removeShellSurface);
    disconnect(shellSurface, &STEShellSurface_wl::resizeRequest, this, &STEAppInstance::resizeRequest);
    shellsurfaces.removeOne(shellSurface);

    if(shellsurfaces.isEmpty())
        deleteLater();
}

void STEAppInstance::addShellSurface(STEShellSurface_wl* newShellsurface)
{
    foreach(STEShellSurface_wl* shellSurface, shellsurfaces)
    {
        if(shellSurface == newShellsurface)
            return;
    }

    connect(newShellsurface, &STEShellSurface_wl::surfaceDestroyed, this, &STEAppInstance::removeShellSurface);
    connect(newShellsurface, &STEShellSurface_wl::resizeRequest, this, &STEAppInstance::resizeRequest);
    shellsurfaces.append(newShellsurface);

    emit shellSurfaceAdded(newShellsurface);
}
