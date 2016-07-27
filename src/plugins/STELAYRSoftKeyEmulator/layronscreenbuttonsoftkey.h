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
    explicit LAYROnscreenButtonSoftKey(QString hint, QObject* parent = 0);

    virtual QString getHint() const Q_DECL_OVERRIDE;
    virtual bool isVisual() const Q_DECL_OVERRIDE;
    virtual QSize getHintSize() const Q_DECL_OVERRIDE;
    virtual void setSurface(STEAppInstance* appInstance, QWaylandSurface* newSurface) Q_DECL_OVERRIDE;

    void setPosition(QPointF pos);
    QQuickItem* item() {return &waylandQuickItem;}

private:
    QWaylandQuickItem waylandQuickItem;
    QPointF pos;
    QString hint;
    QTimer timer;
    QMap<STEAppInstance*, QWaylandSurface*> appSurfacesList;
    STEAppInstance* activeAppInstance;

    void updateLocation();
    void updateActiveSurface();
};

#endif // LAYRONSCREENBUTTONSOFTKEY_H
