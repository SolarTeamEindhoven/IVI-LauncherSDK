#ifndef STESHELL_WL_H
#define STESHELL_WL_H

#include <QObject>

#include <QWaylandCompositorExtensionTemplate>

#include <STE-LauncherSDK/private/qwayland-server-STE.h>

#include "wayland-util.h"

QT_BEGIN_NAMESPACE

class STEShellSurface_wl;
class STESoftKey_wl;

class STEShell_wl : public QWaylandCompositorExtensionTemplate<STEShell_wl>, public QtWaylandServer::ste_shell
{
    Q_OBJECT

public:
    STEShell_wl(QWaylandObject* container);
    ~STEShell_wl();

    Q_INVOKABLE void initialize();

signals:
    void createShellSurface(STEShellSurface_wl* shellSurface);
    void createSoftkey(STESoftKey_wl* softkey);

protected:
    void ste_shell_create_ste_shell_surface(Resource *resource, uint32_t id, struct ::wl_resource *surface) Q_DECL_OVERRIDE;
    void ste_shell_create_ste_softkey(Resource* resource, uint32_t id, uint32_t softkey_id) Q_DECL_OVERRIDE;
    void ste_shell_create_ste_softkey_with_surface(Resource *resource, uint32_t id, uint32_t softkey_id, struct ::wl_resource *surface) Q_DECL_OVERRIDE;

private:
    static STEShell_wl* instance;

    STESoftKey_wl* createSoftKey(Resource* resource, uint32_t id, uint32_t softkey_id);
};

QT_END_NAMESPACE

#endif // STESHELL_WL_H
