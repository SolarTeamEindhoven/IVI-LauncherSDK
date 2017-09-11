#include "iviappcontainer.h"

#include <QtWaylandCompositor/QWaylandSurface>
#include <QtWaylandCompositor/QWaylandIviSurface>
#include <QtWaylandCompositor/QWaylandQuickShellSurfaceItem>

#include <IVI-Launcher/IVISurfaceManager>
#include <IVI-Launcher/IVIApplication>
#include <ivisingletonmanager_p.h>

QT_BEGIN_NAMESPACE

IVIAppContainer::IVIAppContainer(QQuickItem* parent)
    : QQuickItem(parent)
    , application(nullptr)
{
    connect(this, &IVIAppContainer::widthChanged, this, &IVIAppContainer::handleWidthChanged);
    connect(this, &IVIAppContainer::heightChanged, this, &IVIAppContainer::handleHeightChanged);
    IVISingletonManager::getSurfaceManager().registerAppContainer(this);
}

IVIAppContainer::~IVIAppContainer() {
    IVISingletonManager::getSurfaceManager().unregisterAppContainer(this);
}

IVIApplication* IVIAppContainer::getApplication() const {
    return application;
}

void IVIAppContainer::setApplication(IVIApplication* newApplication) {
    if(application == newApplication)
        return;

    if(application != nullptr) {
        disconnect(application, &IVIApplication::newSurface, this, &IVIAppContainer::handleNewSurface);
        foreach(QWaylandQuickShellSurfaceItem* surfaceItem, waylandQuickShellSurfaceItems) {
            surfaceItem->deleteLater();
        }
        waylandQuickShellSurfaceItems.clear();
    }

    application = newApplication;
    connect(application, &IVIApplication::newSurface, this, &IVIAppContainer::handleNewSurface);

    foreach(QWaylandIviSurface* surface, application->surfaces()) {
        handleNewSurface(surface);
    }
}

void IVIAppContainer::handleNewSurface(QWaylandIviSurface* iviSurface) {
    if(iviSurface == nullptr)
        return;

    QWaylandQuickShellSurfaceItem* waylandQuickShellSurfaceItem = new QWaylandQuickShellSurfaceItem(this);
    QObject::connect(waylandQuickShellSurfaceItem, &QWaylandQuickShellSurfaceItem::widthChanged, this, &IVIAppContainer::handleSurfaceResize);
    QObject::connect(waylandQuickShellSurfaceItem, &QWaylandQuickShellSurfaceItem::heightChanged, this, &IVIAppContainer::handleSurfaceResize);
    QObject::connect(waylandQuickShellSurfaceItem, &QWaylandQuickShellSurfaceItem::surfaceDestroyed, this, &IVIAppContainer::handleSurfaceDestroy);
    waylandQuickShellSurfaceItems.append(waylandQuickShellSurfaceItem);
    waylandQuickShellSurfaceItem->setShellSurface(iviSurface);

    iviSurface->sendConfigure(QSize(width(), height()));
}

void IVIAppContainer::handleWidthChanged() {
    foreach(QWaylandQuickShellSurfaceItem* waylandQuickShellSurfaceItem, waylandQuickShellSurfaceItems) {
        if(waylandQuickShellSurfaceItem == nullptr)
            break;
        waylandQuickShellSurfaceItem->setWidth(width());
        QWaylandIviSurface* surface = qobject_cast<QWaylandIviSurface*>(waylandQuickShellSurfaceItem->shellSurface());
        if(surface == nullptr)
            break;
        surface->sendConfigure(QSize(width(), height()));
    }
}

void IVIAppContainer::handleHeightChanged() {
    foreach(QWaylandQuickShellSurfaceItem* waylandQuickShellSurfaceItem, waylandQuickShellSurfaceItems) {
        if(waylandQuickShellSurfaceItem == nullptr)
            break;
        waylandQuickShellSurfaceItem->setHeight(height());
        QWaylandIviSurface* surface = qobject_cast<QWaylandIviSurface*>(waylandQuickShellSurfaceItem->shellSurface());
        if(surface == nullptr)
            break;
        surface->sendConfigure(QSize(width(), height()));
    }
}

void IVIAppContainer::handleSurfaceResize() {
    QWaylandQuickShellSurfaceItem* waylandQuickShellSurfaceItem = qobject_cast<QWaylandQuickShellSurfaceItem*>(sender());
    if (waylandQuickShellSurfaceItem == nullptr)
        return;

    waylandQuickShellSurfaceItem->setX((width() - waylandQuickShellSurfaceItem->width() + 1)/2);
    waylandQuickShellSurfaceItem->setY((height() - waylandQuickShellSurfaceItem->height() + 1)/2);
}

void IVIAppContainer::handleSurfaceDestroy() {
    QWaylandQuickShellSurfaceItem* waylandQuickShellSurfaceItem = qobject_cast<QWaylandQuickShellSurfaceItem*>(sender());
    if (waylandQuickShellSurfaceItem == nullptr)
        return;

    waylandQuickShellSurfaceItem->deleteLater();
    waylandQuickShellSurfaceItems.removeAll(waylandQuickShellSurfaceItem);
}

QT_END_NAMESPACE
