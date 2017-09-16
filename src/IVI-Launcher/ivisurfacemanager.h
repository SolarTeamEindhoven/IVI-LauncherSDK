#ifndef IVISURFACEMANAGER_H
#define IVISURFACEMANAGER_H

#include <QtCore/QObject>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class IVIAbstractAppContainer;
class IVISurfaceManagerPrivate;

class QWaylandIviSurface;
class QWaylandCompositor;

class Q_IVI_LAUNCHER_EXPORT IVISurfaceManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IVISurfaceManager)

public:
    explicit IVISurfaceManager(QWaylandCompositor* compositor, QObject* parent = nullptr);

    void registerAppContainer(IVIAbstractAppContainer* appContainer);
    void unregisterAppContainer(IVIAbstractAppContainer* appContainer);
};

QT_END_NAMESPACE

#endif // IVISURFACEMANAGER_H
