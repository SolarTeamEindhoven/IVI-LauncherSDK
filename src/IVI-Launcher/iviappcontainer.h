#ifndef IVIAPPCONTAINER_H
#define IVIAPPCONTAINER_H

#include <QtQuick/QQuickItem>

#include <IVI-Launcher/ivilauncherglobal.h>
#include <IVI-Launcher/IVIAbstractAppContainer>

QT_BEGIN_NAMESPACE

class QWaylandIviSurface;
class QWaylandQuickShellSurfaceItem;
class IVIApplication;

/*
 * TODO: Convert to widget version
 */

class Q_IVI_LAUNCHER_EXPORT IVIAppContainer : public QQuickItem, public IVIAbstractAppContainer
{
    Q_OBJECT
    Q_PROPERTY(IVIApplication* application READ getApplication WRITE setApplication NOTIFY applicationChanged)

public:
    explicit IVIAppContainer(QQuickItem* parent = nullptr);
    virtual ~IVIAppContainer();

    IVIApplication* getApplication() const;
    void setApplication(IVIApplication*);

    void handleNewSurface(QWaylandIviSurface* iviSurface) override;

signals:
    void applicationChanged();

private:
    QList<QWaylandQuickShellSurfaceItem*> waylandQuickShellSurfaceItems;
    IVIApplication* application;

    void handleWidthChanged();
    void handleHeightChanged();
    void handleSurfaceResize();
    void handleSurfaceDestroy();
};

QT_END_NAMESPACE

#endif // IVIAPPCONTAINER_H
