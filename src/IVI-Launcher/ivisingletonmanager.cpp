#include "ivisingletonmanager_p.h"

#include <memory>
#include <unordered_set>

#include <QtWaylandCompositor/QWaylandTextInputManager>
#include <QWaylandQuickOutput>
#include <QDebug>

#include "qwaylandquickcompositorquickextensioncontainer_p.h"

#include <IVISurfaceManager>
#include "ividbusmanager_p.h"
#include "iviappprocessmanager_p.h"
#include "iviapplicationmanager_p.h"

QT_BEGIN_NAMESPACE

namespace {
enum class CompositorType {
    Invalid,
    Wigets,
    QML
};

struct CompositorWrapper {
    CompositorWrapper(CompositorType type)
        : compositorIsCreated(false)
        , type(type)
        , compositor(createCompositor(type))
        , textInputManager(compositor.get())
        , surfaceManager(compositor.get())
    {}

    bool compositorIsCreated;
    const CompositorType type;
    std::unique_ptr<QWaylandCompositor> compositor;
    QWaylandTextInputManager textInputManager;
    IVISurfaceManager surfaceManager;
    IVIDBusManager dBusManager;
    std::unordered_set<QWindow*> outputWindows;

private:
    static QWaylandCompositor* createCompositor(CompositorType type) {
        switch(type) {
        default:
            qWarning() << "Using compositor before explicitly specifying compositor type. Default to widgets!";
            [[fallthrough]];
        case CompositorType::Wigets:
            return nullptr; // TODO
        case CompositorType::QML:
            {
                QWaylandQuickCompositorQuickExtensionContainer* compositor = new QWaylandQuickCompositorQuickExtensionContainer();
                compositor->create();
                return compositor;
            }
        }
    }
};

static CompositorWrapper& getCompositorWrapper(CompositorType defaultType) {
    static CompositorWrapper compositorWrapper(defaultType);
    return compositorWrapper;
}

}

void IVISingletonManager::createQmlCompositor(QQuickWindow* window) noexcept {
    auto& compositorWrapper = getCompositorWrapper(CompositorType::QML);

    if(window == nullptr)
        return;

    if(compositorWrapper.type != CompositorType::QML)
        return;

    if(compositorWrapper.outputWindows.find(window) == compositorWrapper.outputWindows.end()) {
        QWaylandQuickOutput* output = new QWaylandQuickOutput(compositorWrapper.compositor.get(), window);
        output->setSizeFollowsWindow(true);
        compositorWrapper.compositor;
        compositorWrapper.outputWindows.insert(window);
    }
}

QWaylandCompositor& IVISingletonManager::getCompositor() noexcept {
    auto& compositorWrapper = getCompositorWrapper(CompositorType::Invalid);
    return *compositorWrapper.compositor;
}

IVISurfaceManager& IVISingletonManager::getSurfaceManager() noexcept {
    auto& compositorWrapper = getCompositorWrapper(CompositorType::Invalid);
    return compositorWrapper.surfaceManager;
}

QT_END_NAMESPACE
