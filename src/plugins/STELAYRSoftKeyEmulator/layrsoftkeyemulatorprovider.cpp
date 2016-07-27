#include "layrsoftkeyemulatorprovider.h"

#include <QList>
#include <QWaylandCompositor>
#include <STEAppInstance>

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

void LAYRSoftKeyEmulatorProvider::activeAppChanged(STEAppInstance* appInstance)
{
    getController()->setActiveApp(appInstance);
}

LAYRController* LAYRSoftKeyEmulatorProvider::getController()
{
    if(controller == nullptr)
    {
        controller = new LAYRController;
        connect(controller, &LAYRController::changeActiveApp, this, &LAYRSoftKeyEmulatorProvider::changeActiveApp);
    }

    return controller;
}
