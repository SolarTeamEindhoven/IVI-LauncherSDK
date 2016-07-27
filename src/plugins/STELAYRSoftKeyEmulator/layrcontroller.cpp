#include "layrcontroller.h"

#include <cmath>

#include <QSGNode>
#include <QSGSimpleRectNode>
#include <QQuickWindow>

LAYRController::LAYRController(QQuickItem* parent)
    : QQuickItem(parent)
    , leftButton("LAYR_left")
    , middleButton("LAYR_middle")
    , rightButton("LAYR_right")
    , p(.4f)
    , t(0)
{
    setFlag(ItemHasContents, true);

    timer.setInterval(10);
    connect(&timer, &QTimer::timeout, this, &LAYRController::updateLAYR);
    timer.start();

    connect(this, &QQuickItem::windowChanged, this, &LAYRController::updateWindowSize);
    updateWindowSize();

    leftButton.item()->setParentItem(this);
}

QSGNode* LAYRController::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    QSGSimpleRectNode* node = static_cast<QSGSimpleRectNode*>(oldNode);

    if(node == nullptr)
    {
        node = new QSGSimpleRectNode;
        node->setColor(QColor(100, 100, 100, 100));
    }

    node->setRect(QRectF(0, p * windowSize.height(), windowSize.width(), height()));

    return node;
}

void LAYRController::updateLAYR()
{
    t += .01f;
    p = .4f + .1f * std::sin( t * (.25f*2.f*3.141592f) ); // 10% amplitude @ 0.25 Hz

    updatePositions();
}

void LAYRController::updateWindowSize()
{
    QQuickWindow* w = window();
    QSize newWindowSize;

    if(w == nullptr || w->contentItem() == nullptr)
        newWindowSize = QSize(0, 0);
    else
        newWindowSize = QSize(w->contentItem()->width(), w->contentItem()->height());

    if(windowSize != newWindowSize)
    {
        windowSize = newWindowSize;
        updatePositions();
    }
}

void LAYRController::updatePositions()
{
    int y = p * windowSize.height();

    leftButton.setPosition(QPointF(windowSize.width()/4, y + height()/2));
    middleButton.setPosition(QPointF(2*windowSize.width()/4, y + height()/2));
    rightButton.setPosition(QPointF(3*windowSize.width()/4, y + height()/2));
    update();
}
