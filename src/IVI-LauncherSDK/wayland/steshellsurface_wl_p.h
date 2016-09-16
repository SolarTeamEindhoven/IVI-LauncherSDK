#ifndef STESHELLSURFACE_WL_H
#define STESHELLSURFACE_WL_H

#include <QObject>
#include <QSize>
#include <QTimer>
#include <QDebug>

#include <QWaylandCompositorExtensionTemplate>
#include <QWaylandSurfaceRole>

#include <IVI-LauncherSDK/private/qwayland-server-STE.h>

#include "wayland-util.h"

QT_BEGIN_NAMESPACE

class QWaylandSurface;
class STESoftKey_wl;

class STEShellSurface_wl : public QWaylandCompositorExtensionTemplate<STEShellSurface_wl>, public QtWaylandServer::ste_shell_surface
{
    Q_OBJECT

    Q_ENUMS(state)

public:
    explicit STEShellSurface_wl(QWaylandSurface* surface, wl_resource* resource);
    ~STEShellSurface_wl();

    void initialize(QWaylandSurface* surface, wl_resource* resource);
    QWaylandSurface* getSurface() const { return surface; }

    static QWaylandSurfaceRole& role();

signals:
    void surfaceChanged();
    void surfaceDestroyed();

public slots:
    void setSize(const QSize& size, state s);
    void updateWidth();
    void updateHeight();

protected:
    void ste_shell_surface_resize(Resource* resource, uint32_t state) Q_DECL_OVERRIDE;

private:
    QWaylandSurface* surface;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mState;

    static QWaylandSurfaceRole surfaceRole;

    void sendConfigureUpdate();
    void redraw();
    void damaged(const QRegion &rect);
};

QT_END_NAMESPACE

#endif // STESHELLSURFACE_WL_H
