#ifndef IVISURFACEMANAGER_P_H
#define IVISURFACEMANAGER_P_H

#include <mutex>

#include <QtCore/private/qobject_p.h>
#include <QtWaylandCompositor/QWaylandIviApplication>
#include <IVI-Launcher/ivisurfacemanager.h>

QT_BEGIN_NAMESPACE

class QWaylandIviSurface;

class Q_DECL_EXPORT IVISurfaceManagerPrivate : public QObjectPrivate
{
public:
    explicit IVISurfaceManagerPrivate(QWaylandCompositor*);

    void initialize();

    void registerAppContainer(IVIAbstractAppContainer* appContainer);
    void unregisterAppContainer(IVIAbstractAppContainer* appContainer);

private:
    QWaylandIviApplication waylandIviApplication;
    QList<IVIAbstractAppContainer*> appContainers;

    std::mutex appContainerMapMutex;
    std::multimap<qint64, IVIAbstractAppContainer*> appContainerMap;

    void handleIviSurfaceCreated(QWaylandIviSurface* iviSurface);

private:
    Q_DECLARE_PUBLIC(IVISurfaceManager)
    Q_DISABLE_COPY(IVISurfaceManagerPrivate)
};

QT_END_NAMESPACE

#endif // IVISURFACEMANAGER_P_H
