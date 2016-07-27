#ifndef LAYRONSCREENBUTTONSOFTKEY_H
#define LAYRONSCREENBUTTONSOFTKEY_H

#include <STESoftKey>
#include <QTimer>
#include <QMap>
#include <QWaylandQuickItem>

class LAYRController;
class QQuickWindow;

class LAYROnscreenButtonSoftKey : public STESoftKey
{
    Q_OBJECT

public:
    explicit LAYROnscreenButtonSoftKey(QQuickItem* parentItem, QString hint, QObject* parent = 0);

    virtual QString getHint() const Q_DECL_OVERRIDE;
    virtual bool isVisual() const Q_DECL_OVERRIDE;
    virtual QSize getHintSize() const Q_DECL_OVERRIDE;
    virtual void setSurface(STEAppInstance* appInstance, QWaylandSurface* newSurface) Q_DECL_OVERRIDE;

    void setPosition(QPointF pos);
    void setActiveApp(STEAppInstance* appInstance);

private:
    QQuickItem* parentItem;
    QPointF pos;
    QString hint;
    QTimer timer;
    QMap<STEAppInstance*, QWaylandQuickItem*> appSurfacesList;
    STEAppInstance* activeAppInstance;

    void updateLocation();
};

#endif // LAYRONSCREENBUTTONSOFTKEY_H
