#include "layrsoftkeyemulatorprovider.h"

#include <QList>
#include <QWaylandCompositor>

static QList<STESoftKey*> softKeyList;

LAYRSoftKeyEmulatorProvider::LAYRSoftKeyEmulatorProvider(QObject *parent)
    : controller(nullptr)
{
}

QList<STESoftKey*> LAYRSoftKeyEmulatorProvider::createSoftKeys()
{
    return getController()->getSoftKeyList();
}

QQuickItem* LAYRSoftKeyEmulatorProvider::createSoftKeyVisualization()
{
    return getController();
}

LAYRController* LAYRSoftKeyEmulatorProvider::getController()
{
    if(controller == nullptr)
        controller = new LAYRController;

    return controller;
}
