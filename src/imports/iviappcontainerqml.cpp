#include "iviappcontainerqml.h"

#include <QWaylandIviSurface>
#include <QWaylandQuickShellSurfaceItem>

#include <IVI-Launcher/IVIApplication>
#include <IVI-Launcher/private/ivisingletonmanager_p.h>

QT_BEGIN_NAMESPACE

IVIAppContainerQml::IVIAppContainerQml(QQuickItem* parent)
    : QQuickItem(parent)
    , application(nullptr)
{
    IVISingletonManager::createQmlCompositor(window());
    connect(this, &IVIAppContainerQml::windowChanged, this, &IVIAppContainerQml::handleWindowChanged);

    connect(this, &IVIAppContainerQml::widthChanged, this, &IVIAppContainerQml::handleWidthChanged);
    connect(this, &IVIAppContainerQml::heightChanged, this, &IVIAppContainerQml::handleHeightChanged);
}

IVIAppContainerQml::~IVIAppContainerQml() {
}

IVIApplication* IVIAppContainerQml::getApplication() const {
    return application;
}

void IVIAppContainerQml::setApplication(IVIApplication* newApplication) {
    if(application == newApplication)
        return;

    if(application != nullptr) {
        disconnect(application, &IVIApplication::newSurface, this, &IVIAppContainerQml::handleNewSurface);
        foreach(QWaylandQuickShellSurfaceItem* surfaceItem, waylandQuickShellSurfaceItems) {
            surfaceItem->deleteLater();
        }
        waylandQuickShellSurfaceItems.clear();
    }

    application = newApplication;
    connect(application, &IVIApplication::newSurface, this, &IVIAppContainerQml::handleNewSurface);

    foreach(QWaylandIviSurface* surface, application->surfaces()) {
        handleNewSurface(surface);
    }
}

void IVIAppContainerQml::handleNewSurface(QWaylandIviSurface* iviSurface) {
    if(iviSurface == nullptr)
        return;

    QWaylandQuickShellSurfaceItem* waylandQuickShellSurfaceItem = new QWaylandQuickShellSurfaceItem(this);
    QObject::connect(waylandQuickShellSurfaceItem, &QWaylandQuickShellSurfaceItem::widthChanged, this, &IVIAppContainerQml::handleSurfaceResize);
    QObject::connect(waylandQuickShellSurfaceItem, &QWaylandQuickShellSurfaceItem::heightChanged, this, &IVIAppContainerQml::handleSurfaceResize);
    QObject::connect(waylandQuickShellSurfaceItem, &QWaylandQuickShellSurfaceItem::surfaceDestroyed, this, &IVIAppContainerQml::handleSurfaceDestroy);
    waylandQuickShellSurfaceItems.append(waylandQuickShellSurfaceItem);
    waylandQuickShellSurfaceItem->setShellSurface(iviSurface);

    iviSurface->sendConfigure(QSize(width(), height()));
}

void IVIAppContainerQml::handleWidthChanged() {
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

void IVIAppContainerQml::handleHeightChanged() {
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

void IVIAppContainerQml::handleSurfaceResize() {
    QWaylandQuickShellSurfaceItem* waylandQuickShellSurfaceItem = qobject_cast<QWaylandQuickShellSurfaceItem*>(sender());
    if (waylandQuickShellSurfaceItem == nullptr)
        return;

    waylandQuickShellSurfaceItem->setX((width() - waylandQuickShellSurfaceItem->width() + 1)/2);
    waylandQuickShellSurfaceItem->setY((height() - waylandQuickShellSurfaceItem->height() + 1)/2);
}

void IVIAppContainerQml::handleSurfaceDestroy() {
    QWaylandQuickShellSurfaceItem* waylandQuickShellSurfaceItem = qobject_cast<QWaylandQuickShellSurfaceItem*>(sender());
    if (waylandQuickShellSurfaceItem == nullptr)
        return;

    waylandQuickShellSurfaceItem->deleteLater();
    waylandQuickShellSurfaceItems.removeAll(waylandQuickShellSurfaceItem);
}

void IVIAppContainerQml::handleWindowChanged(QQuickWindow* window) {
    IVISingletonManager::createQmlCompositor(window);
}

QT_BEGIN_NAMESPACE
