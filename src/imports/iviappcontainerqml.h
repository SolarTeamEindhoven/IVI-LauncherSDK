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

    Q_PROPERTY(QString applicationName READ getName NOTIFY applicationChanged)
    Q_PROPERTY(QString applicationDescription READ getDescription NOTIFY applicationChanged)
    Q_PROPERTY(QString applicationIcon READ getIcon NOTIFY applicationChanged)
    Q_PROPERTY(QString applicationWebsite READ getWebsite NOTIFY applicationChanged)
    Q_PROPERTY(QString applicationExecutable READ getExecutable NOTIFY applicationChanged)
    Q_PROPERTY(IVIApplication::RunningState applicationRunningstate READ getRunningState NOTIFY runningStateChanged)

public:
    explicit IVIAppContainerQml(QQuickItem* parent = nullptr);
    virtual ~IVIAppContainerQml();

    IVIApplication* getApplication() const;
    void setApplication(IVIApplication*);

    const QString& getName() const;
    const QString& getDescription() const;
    const QString& getIcon() const;
    const QString& getWebsite() const;
    const QList<QString>& getCategories() const;
    const QString& getExecutable() const;
    IVIApplication::RunningState getRunningState() const;

signals:
    void applicationChanged();
    void runningStateChanged();

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
