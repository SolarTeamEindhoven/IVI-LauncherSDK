#ifndef STEAPPCONTAINER_H
#define STEAPPCONTAINER_H

#include <QQuickItem>
#include <QList>

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STEApp;
class STEAppInstance;
class STEShellSurface_wl;
class QWaylandQuickItem;

class Q_STE_LAUNCHERSDK_EXPORT STEAppContainer : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(STEApp* app READ getApp WRITE setApp NOTIFY appChanged)

public:
    STEAppContainer(QQuickItem* parent = nullptr);
    ~STEAppContainer();

    STEApp* getApp() const { return app; }
    void setApp(STEApp* app);

    static QList<STEAppContainer*>& getSTEAppContainerList();

signals:
    void appChanged();

private slots:
    void addAppInstance(STEAppInstance* appInstance);
    void removeAppInstance();
    void removeAppInstance(STEAppInstance* appInstance);
    void addShellSurface(STEShellSurface_wl* shellSurface);
    void removeShellSurface();
    void removeShellSurface(STEShellSurface_wl* shellSurface);

private:
    STEApp* app;

    QList<QWaylandQuickItem*> waylandItems;
};

QT_END_NAMESPACE

#endif // STEAPPCONTAINER_H
