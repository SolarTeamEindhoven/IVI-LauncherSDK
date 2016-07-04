#include "steappinstance.h"

#include <QList>
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

STEAppInstance::STEAppInstance(STEShellSurface_wl* shellsurface, QObject* parent)
    : STEAppInstance( STEAppManager::getAppFromPID(shellsurface->getSurface()->client()->processId()), parent )
{
    addShellSurface(shellsurface);
}

STEAppInstance::~STEAppInstance()
{
    qDebug() << "Delete app instance at:" << static_cast<void*>(this);
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

STEAppInstance* STEAppInstance::fromPID(qint64 PID)
{
    foreach(STEAppInstance* instance, STEAppInstanceList)
    {
        if(instance->getPID() == PID)
        {
            return instance;
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
        delete this;
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
