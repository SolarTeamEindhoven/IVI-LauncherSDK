#ifndef IVIAPPCONTAINERQML_H
#define IVIAPPCONTAINERQML_H

#include <QQuickItem>
#include <QQuickWindow>
#include <QWaylandQuickShellSurfaceItem>
#include <IVIAbstractAppContainer>
#include <IVIApplication>

class IVIAppContainerQml : public QQuickItem, public IVIAbstractAppContainer
{
    Q_OBJECT
    Q_PROPERTY(IVIApplication* application READ getApplication WRITE setApplication NOTIFY applicationChanged)

public:
    explicit IVIAppContainerQml(QQuickItem* parent = nullptr);
    virtual ~IVIAppContainerQml();

    IVIApplication* getApplication() const;
    void setApplication(IVIApplication*);

signals:
    void applicationChanged();

private:
    QList<QWaylandQuickShellSurfaceItem*> waylandQuickShellSurfaceItems;
    IVIApplication* application;

    void handleNewSurface(QWaylandIviSurface* iviSurface);
    void handleWidthChanged();
    void handleHeightChanged();
    void handleSurfaceResize();
    void handleSurfaceDestroy();
    void handleWindowChanged(QQuickWindow* window);
};

#endif // IVIAPPCONTAINERQML_H
