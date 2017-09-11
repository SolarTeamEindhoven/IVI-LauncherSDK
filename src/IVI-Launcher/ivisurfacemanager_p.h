#ifndef IVISURFACEMANAGER_P_H
#define IVISURFACEMANAGER_P_H

#include <mutex>

#include <QtCore/private/qobject_p.h>
#include <QtWaylandCompositor/QWaylandIviApplication>

QT_BEGIN_NAMESPACE

class IVIAppContainer;
class QWaylandIviSurface;
class IVISurfaceManager;

class Q_DECL_EXPORT IVISurfaceManagerPrivate : public QObjectPrivate
{
public:
    explicit IVISurfaceManagerPrivate(QWaylandCompositor*);

    void initialize();

    void registerAppContainer(IVIAppContainer* appContainer);
    void unregisterAppContainer(IVIAppContainer* appContainer);

private:
    QWaylandIviApplication waylandIviApplication;
    QList<IVIAppContainer*> appContainers;

    std::mutex appContainerMapMutex;
    std::multimap<qint64, IVIAppContainer*> appContainerMap;

    void handleIviSurfaceCreated(QWaylandIviSurface* iviSurface);

private:
    Q_DECLARE_PUBLIC(IVISurfaceManager)
    Q_DISABLE_COPY(IVISurfaceManagerPrivate)
};

QT_END_NAMESPACE

#endif // IVISURFACEMANAGER_P_H
