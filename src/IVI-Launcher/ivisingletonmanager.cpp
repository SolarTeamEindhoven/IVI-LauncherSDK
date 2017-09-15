#include "ivisingletonmanager_p.h"

#include <memory>

#include <QtWaylandCompositor/QWaylandTextInputManager>
#include <QDebug>

#include "qwaylandquickcompositorquickextensioncontainer_p.h"

#include <IVISurfaceManager>
#include "ividbusmanager_p.h"
#include "iviappprocessmanager_p.h"
#include "iviapplicationmanager_p.h"

QT_BEGIN_NAMESPACE

namespace {
class SingletonFactory {
public:
    enum class IVIGraphics {
        UNDEFINED,
        WIDGETS,
        QML
    };

    SingletonFactory(IVIGraphics engine) noexcept
        : engine(toIVIGraphicsEngine(engine))
        , compositor(createCompositor(engine))
        , textInputManager(compositor.get())
        , surfaceManager(compositor.get())
        , applicationManager(IVIApplicationManagerPrivate::createIVIApplicationManagerPrivate())
    {}

    IVIGraphics getEngine() const {
       switch(engine) {
       case IVISingletonManager::IVIGraphicsEngine::WIDGETS:
           return IVIGraphics::WIDGETS;
       case IVISingletonManager::IVIGraphicsEngine::QML:
           return IVIGraphics::QML;
       }
    }

    QWaylandCompositor& getCompositor() const noexcept {
        return *compositor.get();
    }

    IVISurfaceManager& getSurfaceManager() noexcept {
        return surfaceManager;
    }

    IVIApplicationManager& getApplicationManager() noexcept {
        return applicationManager;
    }

    IVIAppProcessManager& getAppProcessManager() noexcept {
        return appProcessManager;
    }

private:
    const IVISingletonManager::IVIGraphicsEngine engine;
    std::unique_ptr<QWaylandCompositor> compositor;
    QWaylandTextInputManager textInputManager;
    IVISurfaceManager surfaceManager;
    IVIDBusManager iviDBusManager;
    IVIApplicationManager& applicationManager;
    IVIAppProcessManager appProcessManager;

    static constexpr IVISingletonManager::IVIGraphicsEngine
    toIVIGraphicsEngine(IVIGraphics graphicsEngine) {
        switch(graphicsEngine) {
        case IVIGraphics::UNDEFINED:
            return IVISingletonManager::IVIGraphicsEngine::WIDGETS;
        case IVIGraphics::WIDGETS:
            return IVISingletonManager::IVIGraphicsEngine::WIDGETS;
        case IVIGraphics::QML:
            return IVISingletonManager::IVIGraphicsEngine::QML;
        }
    }

    static std::unique_ptr<QWaylandCompositor> createCompositor(IVIGraphics engine) {
        if(engine == IVIGraphics::UNDEFINED)
            qWarning() << "Creating Singletons of undefined type! Defaulting to widgets";
        else {
            switch(engine) {
            case IVIGraphics::WIDGETS:
                return std::make_unique<QWaylandQuickCompositorQuickExtensionContainer>(); // TODO
            case IVIGraphics::QML:
                return std::make_unique<QWaylandQuickCompositorQuickExtensionContainer>();
            }
        }
    }
};

static SingletonFactory& getSingletonFactory(SingletonFactory::IVIGraphics engine) {
    static SingletonFactory factory(engine);
    if(engine != SingletonFactory::IVIGraphics::UNDEFINED && factory.getEngine() != engine)
        qFatal("Incorrect singleton initialization!");
    return factory;
}

static inline SingletonFactory& fetchSingletonFactory() {
    return getSingletonFactory(SingletonFactory::IVIGraphics::UNDEFINED);
}
}

bool IVISingletonManager::initialize(IVIGraphicsEngine engine) noexcept {
    switch(engine) {
    case IVIGraphicsEngine::WIDGETS:
        getSingletonFactory(SingletonFactory::IVIGraphics::WIDGETS);
        break;
    case IVIGraphicsEngine::QML:
        getSingletonFactory(SingletonFactory::IVIGraphics::QML);
        break;
    }
    return false;
}

QWaylandCompositor& IVISingletonManager::getCompositor() noexcept {
    return fetchSingletonFactory().getCompositor();
}

IVISurfaceManager& IVISingletonManager::getSurfaceManager() noexcept {
    return fetchSingletonFactory().getSurfaceManager();
}

IVIApplicationManager& IVISingletonManager::getApplicationManager() noexcept {
    return fetchSingletonFactory().getApplicationManager();
}

IVIAppProcessManager& IVISingletonManager::getAppProcessManager() noexcept {
    return fetchSingletonFactory().getAppProcessManager();
}

QT_END_NAMESPACE
