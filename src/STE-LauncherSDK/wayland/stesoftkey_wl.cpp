#include "stesoftkey_wl_p.h"

#include <QQuickView>
#include <QWaylandQuickItem>
#include <QWaylandClient>
#include <QWaylandCompositor>
#include <QDebug>

#include <STE-LauncherSDK/STEAppInstance>
#include <STE-LauncherSDK/STESoftKey>

#include "stesoftkey.h"

static QQuickView* view = nullptr;

QWaylandSurfaceRole STESoftKey_wl::surfaceRole("ste_softkey_surface");

STESoftKey_wl::STESoftKey_wl(STESoftKey* softkey, wl_resource* resource, QObject *parent)
    : QtWaylandServer::ste_softkey(resource)
    , surface(nullptr)
    , softkey(softkey)
{
    if(!softkey->getHintSize().isNull())
        sendSizeHint(softkey->getHintSize());

    connect(softkey, &STESoftKey::triggerStateChange, this, &STESoftKey_wl::state_changed);
    connect(softkey, &STESoftKey::triggerClick, this, &STESoftKey_wl::click);
    connect(softkey, &STESoftKey::triggerRotated, this, &STESoftKey_wl::rotated);
    connect(softkey, &STESoftKey::hintSizeChanged, this, &STESoftKey_wl::sendSizeHint);
}

STESoftKey_wl::~STESoftKey_wl()
{
}

void STESoftKey_wl::setSurface(QWaylandSurface* newSurface)
{
    /*
    // TODO: Check what to do with surface: draw on screen, handle by plugin or invalidate(/ignore)
    if(!softkey->isVisual())
        return;

    surface = newSurface;

    if(waylandQuickItem == nullptr)
    {
        waylandQuickItem = new QWaylandQuickItem();
        waylandQuickItem->setParentItem(view->rootObject());
    }
    else
    {
        if(waylandQuickItem->surface() != nullptr)
            disconnect(waylandQuickItem->surface(), &QWaylandSurface::sizeChanged, this, &STESoftKey_wl::updateSurfacePosition);
    }

    waylandQuickItem->setSurface(surface); // TODO: Check what happens to old surface when we lose sight here...
    updateSurfacePosition();
    connect(surface, &QWaylandSurface::sizeChanged, this, &STESoftKey_wl::updateSurfacePosition);
    //*/

    // TODO: Replace all above with just this...
    qint64 PID = QWaylandClient::fromWlClient(newSurface->compositor(), resource()->client())->processId();
    STEAppInstance* appInstance = STEAppInstance::fromPID(PID);
    softkey->setSurface(appInstance, newSurface);
}

QWaylandSurfaceRole& STESoftKey_wl::role()
{
    return surfaceRole;
}

void STESoftKey_wl::setView(QQuickView* globalView)
{
    view = globalView;
}

void STESoftKey_wl::state_changed(uint32_t state)
{
    send_state_changed(state);
}

void STESoftKey_wl::click()
{
    send_click();
}

void STESoftKey_wl::rotated(int32_t ticks, int32_t angle)
{
    send_rotated(ticks, angle);
}

void STESoftKey_wl::sendSizeHint(QSize size)
{
    send_size_hint(size.width(), size.height());
}
