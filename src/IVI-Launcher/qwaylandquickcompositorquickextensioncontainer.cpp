#include <QtWaylandCompositor/QWaylandQuickCompositor>
#include <QtWaylandCompositor/QWaylandQuickExtension>

QT_BEGIN_NAMESPACE

Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CONTAINER_CLASS(QWaylandQuickCompositor)

// Hack to trigger MOC
namespace {
class Hoi : public QObject {
    Q_OBJECT
public:
    explicit Hoi(QObject* parent = nullptr) : QObject(parent)
    {}
    virtual ~Hoi();
};
}

QT_END_NAMESPACE

#include "qwaylandquickcompositorquickextensioncontainer.moc"
