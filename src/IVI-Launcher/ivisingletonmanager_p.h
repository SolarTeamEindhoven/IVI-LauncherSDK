#ifndef IVISINGLETONMANAGER_H
#define IVISINGLETONMANAGER_H

#include <QtWaylandCompositor/QWaylandCompositor>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class QQuickWindow;

class IVISurfaceManager;
class IVIAppProcessManager;
class IVIApplicationManager;

namespace IVISingletonManager
{
    Q_IVI_LAUNCHER_EXPORT void createQmlCompositor(QQuickWindow*) noexcept;

    QWaylandCompositor& getCompositor() noexcept;
    IVISurfaceManager& getSurfaceManager() noexcept;
};

QT_END_NAMESPACE

#endif // IVISINGLETONMANAGER_H
