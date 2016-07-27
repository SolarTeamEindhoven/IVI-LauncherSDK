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

LAYROnscreenButtonSoftKey::LAYROnscreenButtonSoftKey(QString hint, QObject* parent)
    : pos(0,0)
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
    appSurfacesList.insert(appInstance, surface);
    updateActiveSurface();
}

void LAYROnscreenButtonSoftKey::setPosition(QPointF position)
{
    pos = position;
    updateLocation();
}

void LAYROnscreenButtonSoftKey::updateLocation()
{
    QWaylandSurface* surface = waylandQuickItem.surface();

    QPointF position = pos;

    if(surface != nullptr)
        position = QPointF(
                    position.x() - static_cast<float>(surface->size().width()) / 2.f,
                    position.y() - static_cast<float>(surface->size().height()) / 2.f
                    );

    waylandQuickItem.setPosition(position);
}

void LAYROnscreenButtonSoftKey::updateActiveSurface()
{
    STEAppInstance* newActiveAppInstance = (0 < appSurfacesList.size()) ? appSurfacesList.begin().key() : nullptr; // TODO: Use actual active application instead of first available one...

    if(newActiveAppInstance != activeAppInstance)
    {
        if(waylandQuickItem.surface() != nullptr)
            disconnect(waylandQuickItem.surface(), &QWaylandSurface::sizeChanged, this, &LAYROnscreenButtonSoftKey::updateLocation);

        if(newActiveAppInstance == nullptr)
        {
            waylandQuickItem.setSurface(nullptr);
        }
        else
        {
            QWaylandSurface* surface = appSurfacesList[newActiveAppInstance];

            waylandQuickItem.setSurface(surface);

            if(surface != nullptr)
            {
                connect(surface, &QWaylandSurface::sizeChanged, this, &LAYROnscreenButtonSoftKey::updateLocation);
            }
        }

        updateLocation();
    }
}
