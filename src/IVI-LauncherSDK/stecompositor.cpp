#include "stecompositor.h"

#include <QWaylandQuickOutput>
#include <QQmlContext>
#include <QWaylandClient>
#include <QWaylandQuickItem>

#include "wayland/steshellsurface_wl_p.h"
#include "wayland/stesoftkey_wl_p.h"
#include "wayland/steshell_wl_p.h"

#include "steappcontainer.h"
#include "steappinstance.h"
#include <IVI-LauncherSDK/STESoftKeyManager>
#include <IVI-LauncherSDK/STEAppManager>
#include <IVI-LauncherSDK/STEApp>

#include "stedbusmanager_p.h"

STECompositor::STECompositor(const QUrl& url, QObject *parent)
    : QWaylandQuickCompositor(parent)
    , shell(new STEShell_wl(this))
{
    dbusManager = new STEDBusManager(this);

    STEAppManager manager;
    if(qEnvironmentVariableIsSet("STE_APP_PATH"))
        manager.addAppDirectory(qgetenv("STE_APP_PATH"));

    STESoftKey_wl::setView(&view);

    extensions().append(shell);

    connect(this, &QWaylandCompositor::createSurface, this, &STECompositor::onCreateSurface);
    connect(shell, &STEShell_wl::createShellSurface, this, &STECompositor::onCreateShellSurface);

    create();

    view.setSource(url);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setColor(Qt::black);
    view.create();

    output = new QWaylandQuickOutput(this, &view);

    connect(&view, &QQuickView::afterRendering, output, &QWaylandQuickOutput::sendFrameCallbacks, Qt::DirectConnection);

    view.setTitle(QLatin1String("QML Compositor"));
    view.setGeometry(0, 0, 1024, 768);
    view.rootContext()->setContextProperty("compositor", this);

    view.show();

    QQuickItem* softkeyVisualItem = STESoftKeyManager::instance()->item();
    softkeyVisualItem->setParentItem(view.rootObject());

    foreach(STEApp* app, manager.getAppList())
        app->launch();
}

void STECompositor::onCreateSurface(QWaylandClient* client, uint id, int version)
{
    qDebug() << "Fired onCreateSurface: {client PID:" << client->processId() << ", id:" << id << ", version:" << version << '}';

//    QWaylandQuickItem* n = new QWaylandQuickItem();
}

void STECompositor::onCreateShellSurface(STEShellSurface_wl* shellSurface)
{
    qDebug() << "Process" << shellSurface->getSurface()->client()->processId() << "created shell surface!";

    STEAppInstance* appInstance = STEAppInstance::fromPID(shellSurface->getSurface()->client()->processId()); // TODO: Protect against null pointers

    if(appInstance == nullptr)
    {
        shellSurface->deleteLater();
        return;
    }

    appInstance->addShellSurface(shellSurface);
}
