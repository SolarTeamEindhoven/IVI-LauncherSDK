#include "iviapplicationmanagerqml.h"

#include <IVI-Launcher/private/iviapplicationmanager_p.h>
#include <IVI-Launcher/private/ivisingletonmanager_p.h>

IVIApplicationManagerQML::IVIApplicationManagerQML(QObject *parent)
    : QObject(parent)
{
    IVISingletonManager::createQmlCompositor(nullptr);
    connect(&IVIApplicationManager::getInstance(), &IVIApplicationManager::applicationListChanged, this, &IVIApplicationManagerQML::applicationListChanged);
}

namespace {
static int CountFunction(QQmlListProperty<IVIApplication>*) {
    return IVIApplicationManagerPrivate::getApplications().count();
}

static IVIApplication* AtFunction(QQmlListProperty<IVIApplication>*, int index) {
    return IVIApplicationManagerPrivate::getApplications().at(index);
}
}

QQmlListProperty<IVIApplication> IVIApplicationManagerQML::getApplicationList() const {
    return QQmlListProperty<IVIApplication>(&IVIApplicationManager::getInstance(), nullptr, CountFunction, AtFunction);
}
