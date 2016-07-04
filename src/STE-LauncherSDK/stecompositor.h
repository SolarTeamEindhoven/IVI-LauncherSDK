#ifndef STECOMPOSITOR_H
#define STECOMPOSITOR_H

#include <QWaylandQuickCompositor>
#include <QQuickView>

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STECompositor;
class STECompositorPrivate;
class STEShellSurface_wl;
class STEShell_wl;

class Q_STE_LAUNCHERSDK_EXPORT STECompositor : public QWaylandQuickCompositor
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(STECompositor)

public:
    explicit STECompositor(const QUrl&, QObject* parent = nullptr);

signals:

public slots:

protected:
    void sizeAdjusted();

private:
    QQuickView view;
    QWaylandOutput *output;
    STEShell_wl* shell;

    void onCreateSurface(QWaylandClient* client, uint id, int version);
    void onCreateShellSurface(STEShellSurface_wl* shellSurface);
};

QT_END_NAMESPACE

#endif // STECOMPOSITOR_H
