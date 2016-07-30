#include "steapp.h"

#include "steappinstance.h"
#include "steappinstancemanaged.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>

#include <STE-LauncherSDK/STEAppManager>

STEApp::STEApp(QDir& directory, QObject* parent)
    : QObject(parent)
    , workingDirectory(directory.absolutePath())
    , manifest(directory.filePath("MANIFEST"))
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
    connect(appInstance, &STEAppInstance::destroyed, this, (void (STEApp::*)())&STEApp::removeAppInstance);
    instances.append(appInstance);
    emit appInstanceAdded(appInstance);
}

void STEApp::removeAppInstance(STEAppInstance* appInstance)
{
    instances.removeOne(appInstance);
}

void STEApp::removeAppInstance()
{
    STEAppInstance* appInstance = qobject_cast<STEAppInstance*>(sender());

    if(appInstance == nullptr)
        return;

    removeAppInstance(appInstance);
}
