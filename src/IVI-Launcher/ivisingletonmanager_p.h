#ifndef IVISINGLETONMANAGER_H
#define IVISINGLETONMANAGER_H

#include <QtWaylandCompositor/QWaylandCompositor>

QT_BEGIN_NAMESPACE

class IVISurfaceManager;
class IVIAppProcessManager;
class IVIApplicationManager;

namespace IVISingletonManager
{
    enum class IVIGraphicsEngine {
        WIDGETS,
        QML
    };

    bool initialize(IVIGraphicsEngine engine) noexcept;

    QWaylandCompositor& getCompositor() noexcept;
    IVISurfaceManager& getSurfaceManager() noexcept;
    IVIApplicationManager& getApplicationManager() noexcept;
    IVIAppProcessManager& getAppProcessManager() noexcept;
};

QT_END_NAMESPACE

#endif // IVISINGLETONMANAGER_H
