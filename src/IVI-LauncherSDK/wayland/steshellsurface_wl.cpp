#include "steshellsurface_wl_p.h"

#include <IVI-LauncherSDK/STESoftKeyManager>

#include "stesoftkey_wl_p.h"

#include <QQuickItem>

#include <QDebug>

#include "stesoftkey.h"

QWaylandSurfaceRole STEShellSurface_wl::surfaceRole("ste_shell_surface");

STEShellSurface_wl::STEShellSurface_wl(QWaylandSurface* surface, wl_resource* resource)
    : surface(surface)
    , mWidth(surface->size().width())
    , mHeight(surface->size().height())
    , mState(0)
{
    connect(surface, &QWaylandSurface::surfaceDestroyed, this, &STEShellSurface_wl::deleteLater);

    init(resource);
    setExtensionContainer(surface);
    QWaylandCompositorExtension::initialize();

    foreach(STESoftKey* softkey, STESoftKeyManager::instance()->getSoftkeyList())
    {
        send_broadcast_available_softkey(softkey->getID(), softkey->getHint());
    }
}

STEShellSurface_wl::~STEShellSurface_wl()
{
    qDebug() << "Deleted shell surface!!";
    emit surfaceDestroyed();
}

void STEShellSurface_wl::initialize(QWaylandSurface* surface, wl_resource* resource)
{
    this->surface = surface;

    init(resource);
    setExtensionContainer(surface);

    emit surfaceChanged();

    QWaylandCompositorExtension::initialize();
}

QWaylandSurfaceRole& STEShellSurface_wl::role()
{
    return surfaceRole;
}

void STEShellSurface_wl::setSize(const QSize& size, sizeState s)
{
    mWidth = size.width();
    mHeight = size.height();
    mState = s;

    sendConfigureUpdate();
}

void STEShellSurface_wl::ste_shell_surface_resize(Resource* resource, uint32_t state)
{
    qDebug() << "Received resize request: " << state;

    // TODO
}

void STEShellSurface_wl::updateWidth()
{
    QQuickItem* quickItem = qobject_cast<QQuickItem*>(sender());

    if(quickItem == nullptr)
        return;

    uint32_t newWidth = quickItem->width();

    if(mWidth == newWidth)
        return;

    mWidth = newWidth;

    sendConfigureUpdate();
}

void STEShellSurface_wl::updateHeight()
{
    QQuickItem* quickItem = qobject_cast<QQuickItem*>(sender());

    if(quickItem == nullptr)
        return;

    uint32_t newHeight = quickItem->height();

    if(mHeight == newHeight)
        return;

    mHeight = newHeight;

    sendConfigureUpdate();
}

void STEShellSurface_wl::sendConfigureUpdate()
{
    if(mWidth == 0 || mHeight == 0)
        return;

    send_configure(mWidth, mHeight, mState);
}
