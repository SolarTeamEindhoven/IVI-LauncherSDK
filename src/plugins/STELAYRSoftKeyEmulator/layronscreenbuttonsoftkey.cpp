#include "layronscreenbuttonsoftkey.h"

#include "layrcontroller.h"

#include <QPoint>
#include <QQmlEngine>
#include <QQuickView>
#include <QQuickWidget>
#include <QWaylandCompositor>
#include <QQmlComponent>

#include <STEAppInstance>

#include <QOpenGLFunctions>

LAYROnscreenButtonSoftKey::LAYROnscreenButtonSoftKey(QQuickItem* parentItem, QString hint, QObject* parent)
    : parentItem(parentItem)
    , pos(0,0)
    , STESoftKey(parent)
    , hint(hint)
    , activeAppInstance(nullptr)
{
    timer.setInterval(1000 + (qrand()/(RAND_MAX/1000)));
    connect(&timer, &QTimer::timeout, this, &LAYROnscreenButtonSoftKey::triggerClick);
    timer.start();
}

QString LAYROnscreenButtonSoftKey::getHint() const
{
    return hint;
}

bool LAYROnscreenButtonSoftKey::isVisual() const
{
    return true;
}

QSize LAYROnscreenButtonSoftKey::getHintSize() const
{
    return QSize(100, 100);
}

void LAYROnscreenButtonSoftKey::setSurface(STEAppInstance* appInstance, QWaylandSurface* surface)
{
    auto it = appSurfacesList.find(appInstance);

    if(it == appSurfacesList.end())
    {
        QWaylandQuickItem* waylandQuickItem = new QWaylandQuickItem();
        waylandQuickItem->setParentItem(parentItem);
        waylandQuickItem->setVisible(false);
        it = appSurfacesList.insert(appInstance, waylandQuickItem);
    }

    it.value()->setSurface(surface);
    setActiveApp(activeAppInstance);
}

void LAYROnscreenButtonSoftKey::setPosition(QPointF position)
{
    pos = position;
    updateLocation();
}

void LAYROnscreenButtonSoftKey::setActiveApp(STEAppInstance* newActiveAppInstance)
{
    if(newActiveAppInstance != activeAppInstance)
    {
        if(activeAppInstance != nullptr)
        {
            QWaylandQuickItem* waylandQuickItem = appSurfacesList[activeAppInstance];
            if(waylandQuickItem->surface() != nullptr)
                disconnect(waylandQuickItem->surface(), &QWaylandSurface::sizeChanged, this, &LAYROnscreenButtonSoftKey::updateLocation);
            waylandQuickItem->setVisible(false);
        }

        if(newActiveAppInstance != nullptr)
        {
            auto it = appSurfacesList.find(newActiveAppInstance);

            if(it == appSurfacesList.end())
            {
                activeAppInstance = nullptr;
            }
            else
            {
                activeAppInstance = newActiveAppInstance;
                QWaylandQuickItem* waylandQuickItem = it.value();
                if(waylandQuickItem->surface() != nullptr)
                    connect(waylandQuickItem->surface(), &QWaylandSurface::sizeChanged, this, &LAYROnscreenButtonSoftKey::updateLocation);
                waylandQuickItem->setVisible(true);
            }
        }
        else
        {
            activeAppInstance = nullptr;
        }

        updateLocation();
    }
}

void LAYROnscreenButtonSoftKey::updateLocation()
{
    auto it = appSurfacesList.find(activeAppInstance);

    QPointF position = pos;

    if(it != appSurfacesList.end())
    {
        QWaylandSurface* surface = it.value()->surface();

        if(surface != nullptr)
            position = QPointF(
                        position.x() - static_cast<float>(surface->size().width()) / 2.f,
                        position.y() - static_cast<float>(surface->size().height()) / 2.f
                        );

        it.value()->setPosition(position);
    }
}
