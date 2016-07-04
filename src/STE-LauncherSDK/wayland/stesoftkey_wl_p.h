#ifndef STESOFTKEY_WL_H
#define STESOFTKEY_WL_H

#include <QWaylandCompositorExtensionTemplate>
#include <QWaylandSurfaceRole>

#include <STE-LauncherSDK/private/qwayland-server-STE.h>

QT_BEGIN_NAMESPACE

class QQuickView;
class QWaylandSurface;
class QWaylandQuickItem;
class STESoftKey;

class STESoftKey_wl : public QWaylandCompositorExtensionTemplate<STESoftKey_wl>, public QtWaylandServer::ste_softkey
{
    Q_OBJECT

public:
    explicit STESoftKey_wl(STESoftKey* softkey, wl_resource* resource, QObject *parent = 0);
    ~STESoftKey_wl();

    QWaylandSurface* getSurface() { return surface; }
    void setSurface(QWaylandSurface* surface);

    static QWaylandSurfaceRole& role();
    static void setView(QQuickView* view);

signals:

public slots:
    void state_changed(uint32_t state);
    void click();
    void rotated(int32_t ticks, int32_t angle);
    void sendSizeHint(QSize size);

private:
    STESoftKey* softkey;
    QWaylandSurface* surface;
    QWaylandQuickItem* waylandQuickItem;

    void updateSurfacePosition();

    static QWaylandSurfaceRole surfaceRole;
};

QT_END_NAMESPACE

#endif // STESOFTKEY_WL_H
