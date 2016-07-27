#ifndef LAYRCONTROLLER_H
#define LAYRCONTROLLER_H

#include <QQuickItem>
#include <QTimer>
#include <QList>

#include "layronscreenbuttonsoftkey.h"

class LAYRController : public QQuickItem
{
    Q_OBJECT

public:
    explicit LAYRController(QQuickItem* parent = nullptr);

    float height() const {return 100;}

    QList<STESoftKey*> getSoftKeyList()
    {
        QList<STESoftKey*> list;
        list << &leftButton;
        list << &middleButton;
        list << &rightButton;
        return list;
    }

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) Q_DECL_OVERRIDE;

private:
    QSize windowSize;
    LAYROnscreenButtonSoftKey leftButton;
    LAYROnscreenButtonSoftKey middleButton;
    LAYROnscreenButtonSoftKey rightButton;
    float p;
    QTimer timer;
    float t;

    void updateLAYR();
    void updateWindowSize();
    void updatePositions();
};

#endif // LAYRCONTROLLER_H
