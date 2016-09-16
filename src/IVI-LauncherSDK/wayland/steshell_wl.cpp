#include "steshell_wl_p.h"

#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandSurface>

#include <IVI-LauncherSDK/STESoftKeyManager>

#include <QDebug>

#include "steshellsurface_wl_p.h"
#include "stesoftkey_wl_p.h"
#include "stesoftkey.h"

STEShell_wl::STEShell_wl(QWaylandObject* container)
    : QWaylandCompositorExtensionTemplate<STEShell_wl>(container)
{
    ;
}

STEShell_wl::~STEShell_wl()
{
    ;
}

void STEShell_wl::initialize()
{
    QWaylandCompositorExtensionTemplate::initialize();

    QWaylandCompositor* compositor = static_cast<QWaylandCompositor*>(extensionContainer());

    if (!compositor)
    {
        qWarning() << "Failed to find QWaylandCompositor when initializing QWaylandShell";
        return;
    }

    init(compositor->display(), 1);
}

void STEShell_wl::ste_shell_create_ste_shell_surface(Resource *resource, uint32_t id, struct ::wl_resource *surface_res)
{
    QWaylandSurface* surface = QWaylandSurface::fromResource(surface_res);

    wl_resource* res = wl_resource_create(resource->client(), &ste_shell_surface_interface, wl_resource_get_version(resource->handle), id);

    // Something with error and wayland 1.4 vs 1.7
    wl_resource* displayRes = wl_client_get_object(resource->client(), 1);

    if (surface->setRole(&STEShellSurface_wl::role(), displayRes, WL_DISPLAY_ERROR_INVALID_OBJECT))
    {
        // TODO: Check if exsisting STEShellSurface_wl can be used...
        STEShellSurface_wl* shellSurface = new STEShellSurface_wl(surface, res);

        emit createShellSurface(shellSurface);
    }
}

void STEShell_wl::ste_shell_create_ste_softkey(Resource* resource, uint32_t id, uint32_t softkey_id)
{
    STESoftKey_wl* softkey_wl = createSoftKey(resource, id, softkey_id);
}

void STEShell_wl::ste_shell_create_ste_softkey_with_surface(Resource* resource, uint32_t id, uint32_t softkey_id, struct ::wl_resource* surface_res)
{
    STESoftKey_wl* softkey_wl = createSoftKey(resource, id, softkey_id);

    if(softkey_wl == nullptr)
        return;

    QWaylandSurface* surface = QWaylandSurface::fromResource(surface_res);

    // Something with error and wayland 1.4 vs 1.7
    wl_resource *displayRes = wl_client_get_object(resource->client(), 1);

    if (surface->setRole(&STESoftKey_wl::role(), displayRes, WL_DISPLAY_ERROR_INVALID_OBJECT))
    {
        softkey_wl->setSurface(surface);
    }
}

STESoftKey_wl* STEShell_wl::createSoftKey(Resource* resource, uint32_t id, uint32_t softkey_id)
{
    STESoftKeyManager* softKeyManager = STESoftKeyManager::instance();

    const QList<STESoftKey*>& softkeyList = softKeyManager->getSoftkeyList();
    if(softkeyList.size() <= softkey_id)
        return nullptr;

    STESoftKey* softkey = softKeyManager->fromID(softkey_id);//*(softkeyList.begin() + softkey_id);

    if( softkey == NULL )
    {
        return nullptr;
    }

    wl_resource* res = wl_resource_create(resource->client(), &ste_softkey_interface, wl_resource_get_version(resource->handle), id);
    STESoftKey_wl* softkey_wl = new STESoftKey_wl(softkey, res);

    emit createSoftkey(softkey_wl);

    return softkey_wl;
}
