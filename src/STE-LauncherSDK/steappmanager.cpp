#include "steappmanager.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QList>
#include <QHash>
#include <QDir>
#include <QDirIterator>

#include "steapp.h"

QList<STEApp*> STEAppManager::apps;
QList<QDir> STEAppManager::appDirs;
QList<STEAppManager*> appManagerList;

STEAppManager::STEAppManager(QObject* parent)
    : QObject(parent)
{
    appManagerList.append(this);
}

STEAppManager::~STEAppManager()
{
    appManagerList.removeOne(this);
}

void STEAppManager::addAppDirectory(const QString& directoryName)
{
    QDir directory(directoryName);
    directory.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    if(appDirs.contains(directory))
        return;

    appDirs.append(directory);

    qDebug() << "Searching subdirectories of directory: " << directory.absolutePath();
    QDirIterator it(directory, QDirIterator::FollowSymlinks);
    while(it.hasNext())
    {
        QDir dir(it.next());

        if(!dir.exists("MANIFEST"))
            continue;

        STEApp* app = new STEApp(dir);
    }

    emit appDirectoriesListChanged();
}

static QString getExecutablePath(quint64 PID)
{
    QString filename = QString("/proc/").append(QString::number(PID)).append("/exe");
    QFileInfo info(filename);
    return info.readLink();
}

void STEAppManager::registerApp(STEApp* app)
{
    if(apps.contains(app))
        return;

    apps.append(app);

    foreach(STEAppManager* appManager, appManagerList)
    {
        qDebug() << "Emitting app changes!!!";
        emit appManager->appListChanged();
    }
}

QQmlListProperty<STEApp> STEAppManager::getAppQMLList()
{
    return QQmlListProperty<STEApp>(this, NULL, &STEAppManager::QMLAppListCountFunction, &STEAppManager::QMLAppListAtFunction);
}

int STEAppManager::QMLAppListCountFunction(QQmlListProperty<STEApp>* list)
{
    return apps.count();
}

STEApp* STEAppManager::QMLAppListAtFunction(QQmlListProperty<STEApp>* list, int index)
{
    return apps.at(index);
}
