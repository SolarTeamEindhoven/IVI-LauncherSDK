#include "ivilauncherqml_p_p.h"

#include <QtWaylandCompositor/QWaylandTextInputManager>
#include <QtWaylandCompositor/QWaylandQuickOutput>

#include <IVISurfaceManager>
#include <ivisingletonmanager_p.h>

QT_BEGIN_NAMESPACE

/*
IVILauncherQML::IVILauncherQML(QWindow* parent)
    : QQuickWindow(*new IVILauncherQMLPrivate(this), parent)
{}

IVILauncherQMLPrivate::IVILauncherQMLPrivate(IVILauncherQML* iviLauncherQML)
    : output(getCompositor(), iviLauncherQML)
{
    output.setSizeFollowsWindow(true);
}

QWaylandCompositor* IVILauncherQMLPrivate::getCompositor() {
    IVISingletonManager::initialize(IVISingletonManager::IVIGraphicsEngine::QML);
    return &IVISingletonManager::getCompositor();
}
*/

QT_END_NAMESPACE
