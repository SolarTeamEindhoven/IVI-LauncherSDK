#include "steappcontainer.h"

#include <QWaylandQuickItem>
#include <QSGClipNode>
#include <QQmlEngine>
#include <QDebug>

#include "wayland/steshellsurface_wl_p.h"
#include "steapp.h"
#include "steappinstance.h"
#include "stecompositor.h"

static QList<STEAppContainer*> STEAppContainerList;

STEAppContainer::STEAppContainer(QQuickItem* parent)
    : QQuickItem(parent)
    , app(nullptr)
{
    STEAppContainerList.append(this);
    setFlag(QQuickItem::ItemHasContents);
    setVisible(true);
}

STEAppContainer::~STEAppContainer()
{
    STEAppContainerList.removeOne(this);
}

QList<STEAppContainer*>& STEAppContainer::getSTEAppContainerList()
{
    return STEAppContainerList;
}

void STEAppContainer::setApp(STEApp* newApp)
{
    if(app == newApp)
        return;

    if(app != nullptr)
    {
        disconnect(app, &STEApp::appInstanceAdded, this, &STEAppContainer::addAppInstance);
        foreach(STEAppInstance* appInstance, app->getInstances())
        {
            removeAppInstance(appInstance);
        }
    }

    foreach(QWaylandQuickItem* waylandItem, waylandItems)
    {
        waylandItem->deleteLater();
    }

    app = newApp;

    connect(app, &STEApp::appInstanceAdded, this, &STEAppContainer::addAppInstance);
    connect(app, &STEApp::appInstanceRemoved, this, &STEAppContainer::removeAppInstance);
    foreach(STEAppInstance* appInstance, app->getInstances())
    {
        addAppInstance(appInstance);
    }

    emit appChanged();
}

void STEAppContainer::addAppInstance(STEAppInstance* appInstance)
{
    connect(appInstance, &STEAppInstance::shellSurfaceAdded, this, &STEAppContainer::addShellSurface);
    foreach(STEShellSurface_wl* shellSurface, appInstance->getShellsurfaces())
    {
        addShellSurface(shellSurface);
    }
}

void STEAppContainer::removeAppInstance(STEAppInstance* appInstance)
{
    disconnect(appInstance, &STEAppInstance::shellSurfaceAdded, this, &STEAppContainer::addShellSurface);
    foreach(STEShellSurface_wl* shellSurface, appInstance->getShellsurfaces())
    {
        removeShellSurface(shellSurface);
    }
}

void STEAppContainer::addShellSurface(STEShellSurface_wl* shellSurface)
{
    connect(shellSurface, &STEShellSurface_wl::surfaceDestroyed, this, (void (STEAppContainer::*)())&STEAppContainer::removeShellSurface);

    QWaylandQuickItem* waylandQuickItem = new QWaylandQuickItem();

    if(waylandQuickItem == nullptr)
        return;

    connect(waylandQuickItem, &QWaylandQuickItem::widthChanged, shellSurface, &STEShellSurface_wl::updateWidth);
    connect(waylandQuickItem, &QWaylandQuickItem::heightChanged, shellSurface, &STEShellSurface_wl::updateHeight);

    waylandQuickItem->setParentItem(this);

    waylandQuickItem->setSurface(shellSurface->getSurface());
    QObject* anchors = waylandQuickItem->property("anchors").value<QObject*>();
    anchors->setProperty("fill", QVariant::fromValue<QQuickItem*>(this));

    waylandItems.append(waylandQuickItem);

    update();
}

void STEAppContainer::removeShellSurface()
{
    STEShellSurface_wl* shellSurface = qobject_cast<STEShellSurface_wl*>(sender());

    if(shellSurface == nullptr)
        return;

    removeShellSurface(shellSurface);
}

void STEAppContainer::removeShellSurface(STEShellSurface_wl* shellSurface)
{
    QWaylandSurface* surface = shellSurface->getSurface();

    for(auto it = waylandItems.begin(); it != waylandItems.end();)
    {
        QWaylandQuickItem* waylandQuickItem = *it;
        if(waylandQuickItem->surface() == surface || waylandQuickItem->surface() == nullptr)
        {
            disconnect(waylandQuickItem, &QWaylandQuickItem::widthChanged, shellSurface, &STEShellSurface_wl::updateWidth);
            disconnect(waylandQuickItem, &QWaylandQuickItem::heightChanged, shellSurface, &STEShellSurface_wl::updateHeight);

            if(waylandQuickItem->surface() != nullptr)
                waylandQuickItem->setSurface(nullptr);

            waylandQuickItem->deleteLater();
            it = waylandItems.erase(it);
        }
        else
        {
            it++;
        }
    }
}
