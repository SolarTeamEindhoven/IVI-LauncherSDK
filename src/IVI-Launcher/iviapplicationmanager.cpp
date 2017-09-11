#include "iviapplicationmanager.h"

#include <QDebug>

QT_BEGIN_NAMESPACE

namespace {
static QList<IVIApplication*> applications;

static int CountFunction(QQmlListProperty<IVIApplication>*) {
    return applications.count();
}

static IVIApplication* AtFunction(QQmlListProperty<IVIApplication>*, int index) {
    return applications.at(index); // TODO: Unsafe!!
}

static QList<IVIApplicationManager*> applicationManagers;
}

IVIApplicationManager::IVIApplicationManager(QObject* parent)
    : QObject(parent)
{
    applicationManagers.append(this);
}

IVIApplicationManager::~IVIApplicationManager() {
    applicationManagers.removeOne(this);
}

QQmlListProperty<IVIApplication> IVIApplicationManager::getQmlApplicationList() const {
    return QQmlListProperty<IVIApplication>(const_cast<IVIApplicationManager*>(this), nullptr, CountFunction, AtFunction);
}

void IVIApplicationManager::registerApplication(IVIApplication* application) {
    applications.append(application); // TODO: Unsafe!
    foreach(IVIApplicationManager* applicationManager, applicationManagers) {
        emit applicationManager->applicationListChanged();
    }
}

QT_END_NAMESPACE
