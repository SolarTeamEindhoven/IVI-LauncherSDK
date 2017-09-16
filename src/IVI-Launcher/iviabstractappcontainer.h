#ifndef IVIABSTRACTAPPCONTAINER_H
#define IVIABSTRACTAPPCONTAINER_H

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class QWaylandIviSurface;

class Q_IVI_LAUNCHER_EXPORT IVIAbstractAppContainer
{
public:
    IVIAbstractAppContainer();
    virtual ~IVIAbstractAppContainer();

    virtual void handleNewSurface(QWaylandIviSurface* iviSurface) = 0;
};

QT_END_NAMESPACE

#endif // IVIABSTRACTAPPCONTAINER_H
