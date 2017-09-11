#ifndef IVIAPPCONTAINER_P_H
#define IVIAPPCONTAINER_P_H

#include <QtQuick/private/qquickitem_p.h>

QT_BEGIN_NAMESPACE

class QWaylandIviSurface;
class QWaylandQuickShellSurfaceItem;
class IVIAppContainer;

class IVIAppContainerPrivate : public QQuickItemPrivate
{
public:
    explicit IVIAppContainerPrivate(IVIAppContainer*);


private:
    QList<QWaylandQuickShellSurfaceItem*> waylandQuickShellSurfaceItems;

    Q_DECLARE_PUBLIC(IVIAppContainer)
    Q_DISABLE_COPY(IVIAppContainerPrivate)
};

QT_END_NAMESPACE

#endif // IVIAPPCONTAINER_P_H
