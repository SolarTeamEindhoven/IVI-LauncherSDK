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
        disconnect(application, &IVIApplication::runningStateChanged, this, &IVIAppContainerQml::runningStateChanged);
        foreach(QWaylandQuickShellSurfaceItem* surfaceItem, waylandQuickShellSurfaceItems) {
            surfaceItem->deleteLater();
        }
        waylandQuickShellSurfaceItems.clear();
    }

    application = newApplication;
    connect(application, &IVIApplication::newSurface, this, &IVIAppContainerQml::handleNewSurface);
    connect(application, &IVIApplication::runningStateChanged, this, &IVIAppContainerQml::runningStateChanged);

    foreach(QWaylandIviSurface* surface, application->surfaces()) {
        handleNewSurface(surface);
    }

    emit applicationChanged();
    emit runningStateChanged();
}

namespace {
    static QString emptyString;
    static QList<QString> emptyCategories;
}

const QString& IVIAppContainerQml::getName() const {
    if(application == nullptr)
        return emptyString;
    return application->getName();
}

const QString& IVIAppContainerQml::getDescription() const {
    if(application == nullptr)
        return emptyString;
    return application->getDescription();
}

const QString& IVIAppContainerQml::getIcon() const {
    if(application == nullptr)
        return emptyString;
    return application->getIcon();
}

const QString& IVIAppContainerQml::getWebsite() const {
    if(application == nullptr)
        return emptyString;
    return application->getWebsite();
}

const QList<QString>& IVIAppContainerQml::getCategories() const {
    if(application == nullptr)
        return emptyCategories;
    return application->getCategories();
}

const QString& IVIAppContainerQml::getExecutable() const {
    if(application == nullptr)
        return emptyString;
    return application->getExecutable();
}

IVIApplication::RunningState IVIAppContainerQml::getRunningState() const {
    if(application == nullptr)
        return IVIApplication::RunningState::NotRunning;
    return application->getRunningState();
}

void IVIAppContainerQml::handleNewSurface(QWaylandIviSurface* iviSurface) {
    if(iviSurface == nullptr)
        return;
    qDebug() << "Adding surface to application container!";

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
