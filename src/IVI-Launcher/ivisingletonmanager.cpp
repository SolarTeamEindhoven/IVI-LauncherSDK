#include "ivisingletonmanager_p.h"

#include <memory>

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
        : type(type)
        , compositor(createCompositor(type))
        , textInputManager(compositor.get())
        , surfaceManager(compositor.get())
    {}

    const CompositorType type;
    std::unique_ptr<QWaylandCompositor> compositor;
    QWaylandTextInputManager textInputManager;
    IVISurfaceManager surfaceManager;

private:
    static QWaylandCompositor* createCompositor(CompositorType type) {
        switch(type) {
        default:
            qWarning() << "Using compositor before explicitly specifying compositor type. Default to widgets!";
            [[fallthrough]];
        case CompositorType::Wigets:
            return nullptr; // TODO
        case CompositorType::QML:
            return new QWaylandQuickCompositorQuickExtensionContainer();
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

    if(compositorWrapper.compositor->outputFor(window) == nullptr) {
        QWaylandQuickOutput* output = new QWaylandQuickOutput(compositorWrapper.compositor.get(), window);
        output->setSizeFollowsWindow(true);
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
