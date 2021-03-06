#include "steapp.h"

#include "steappinstance.h"
#include "steappinstancemanaged.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>

#include <IVI-LauncherSDK/STEAppManager>

STEApp::STEApp(QDir& directory, QObject* parent)
    : QObject(parent)
    , workingDirectory(directory.absolutePath())
    , manifest(directory.filePath(QStringLiteral("MANIFEST")))
{
    qDebug() << "Managed to load app: " << manifest.getName() << "at location:" << static_cast<void*>(this);
    STEAppManager::registerApp(this);
}

STEApp::~STEApp()
{
    qDebug() << "Delete app at location:" << static_cast<void*>(this);
}

void STEApp::launch()
{
    STEAppInstanceManaged* appInstance = new STEAppInstanceManaged(this, this);

    if(appInstance == nullptr)
        return;

    addAppInstance(appInstance);
}

void STEApp::addAppInstance(STEAppInstance* appInstance)
{
    foreach(STEAppInstance* instance, instances)
    {
        if(instance == appInstance)
            return;
    }

    qDebug() << "Adding app instance!";
    connect(appInstance, &STEAppInstance::destroyed, this, static_cast<void (STEApp::*)()>(&STEApp::removeAppInstance));
    connect(appInstance, &STEAppInstance::resizeRequest, this, &STEApp::resizeRequest);
    instances.append(appInstance);
    emit appInstanceAdded(appInstance);
}

void STEApp::removeAppInstance(STEAppInstance* appInstance)
{
    disconnect(appInstance, &STEAppInstance::destroyed, this, static_cast<void (STEApp::*)()>(&STEApp::removeAppInstance));
    disconnect(appInstance, &STEAppInstance::resizeRequest, this, &STEApp::resizeRequest);
    instances.removeOne(appInstance);
    emit appInstanceRemoved(appInstance);
}

void STEApp::removeAppInstance()
{
    STEAppInstance* appInstance = qobject_cast<STEAppInstance*>(sender());

    if(appInstance == nullptr)
        return;

    removeAppInstance(appInstance);
}

QQmlListProperty<const QString> STEApp::getCategoriesQml() const
{
    return QQmlListProperty<const QString>(const_cast<STEApp*>(this), nullptr, &STEApp::categoriesCountFunction, &STEApp::categoriesAtFunction);
}

int STEApp::categoriesCountFunction(QQmlListProperty<const QString>* list)
{
    STEApp* app = qobject_cast<STEApp*>(list->object);
    return app->manifest.getCategories().count();
}

const QString* STEApp::categoriesAtFunction(QQmlListProperty<const QString>* list, int index)
{
    STEApp* app = qobject_cast<STEApp*>(list->object);
    return &(app->manifest.getCategories().at(index));
}
