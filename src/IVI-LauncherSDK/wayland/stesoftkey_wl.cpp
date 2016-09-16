#include "stesoftkey_wl_p.h"

#include <QQuickView>
#include <QWaylandQuickItem>
#include <QWaylandClient>
#include <QWaylandCompositor>
#include <QDebug>

#include <IVI-LauncherSDK/STEAppInstance>
#include <IVI-LauncherSDK/STESoftKey>

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
    disconnect(softkey, &STESoftKey::triggerStateChange, this, &STESoftKey_wl::state_changed);
    disconnect(softkey, &STESoftKey::triggerClick, this, &STESoftKey_wl::click);
    disconnect(softkey, &STESoftKey::triggerRotated, this, &STESoftKey_wl::rotated);
    disconnect(softkey, &STESoftKey::hintSizeChanged, this, &STESoftKey_wl::sendSizeHint);
}

void STESoftKey_wl::setSurface(QWaylandSurface* newSurface)
{
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

void STESoftKey_wl::ste_softkey_destroy_resource(Resource *resource)
{
    deleteLater();
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
