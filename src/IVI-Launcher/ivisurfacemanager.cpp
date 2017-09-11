#include "ivisurfacemanager.h"
#include "ivisurfacemanager_p.h"

#include <QtWaylandCompositor/QWaylandSurface>
#include <QtWaylandCompositor/QWaylandIviSurface>

#include <IVI-Launcher/IVIAppContainer>
#include <IVI-Launcher/IVIApplication>
#include <IVI-Launcher/private/iviapplication_p.h>
#include "ivisingletonmanager_p.h"

#include <QDebug>

QT_BEGIN_NAMESPACE

IVISurfaceManager::IVISurfaceManager(QWaylandCompositor* compositor, QObject* parent)
    : QObject(*new IVISurfaceManagerPrivate(compositor), parent)
{
    Q_D(IVISurfaceManager);
    d->initialize();
}

IVISurfaceManagerPrivate::IVISurfaceManagerPrivate(QWaylandCompositor* compositor)
    : waylandIviApplication(compositor)
{}

void IVISurfaceManagerPrivate::initialize() {
    connect(&waylandIviApplication, &QWaylandIviApplication::iviSurfaceCreated, this, &IVISurfaceManagerPrivate::handleIviSurfaceCreated);
}

void IVISurfaceManager::registerAppContainer(IVIAppContainer* appContainer) {
    Q_D(IVISurfaceManager);
    d->registerAppContainer(appContainer);
}

void IVISurfaceManager::unregisterAppContainer(IVIAppContainer* appContainer) {
    Q_D(IVISurfaceManager);
    d->unregisterAppContainer(appContainer);
}

void IVISurfaceManagerPrivate::registerAppContainer(IVIAppContainer* appContainer) {
    appContainers.append(appContainer);
}

void IVISurfaceManagerPrivate::unregisterAppContainer(IVIAppContainer* appContainer) {
    appContainers.removeAll(appContainer);
}

void IVISurfaceManagerPrivate::handleIviSurfaceCreated(QWaylandIviSurface* iviSurface) {
    auto surface = iviSurface->surface();
    if(surface == nullptr)
        return;

    auto client = surface->client();
    if(client == nullptr)
        return;

    IVIApplication* application = IVIApplication::fromWaylandClient(client);

    if(application == nullptr)
        return;

    IVIApplicationPrivate* app = static_cast<IVIApplicationPrivate*>(IVIApplicationPrivate::get(application));
    app->addSurface(iviSurface);
}

QT_END_NAMESPACE
