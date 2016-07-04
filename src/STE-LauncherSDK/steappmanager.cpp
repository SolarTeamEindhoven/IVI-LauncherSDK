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

STEAppManager::STEAppManager(QObject* parent)
    : QObject(parent)
{
    ;
}

STEAppManager::~STEAppManager()
{
    ;
}

void STEAppManager::addKnownApp(const QString& identifier)
{
//    apps.insert(identifier, new STEApp());
//    emit appListChanged();
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
        apps.append(app);
//        apps.insert(dir.dirName(), app);
    }

    emit appDirectoriesListChanged();
}

static QString getExecutablePath(quint64 PID)
{
    QString filename = QString("/proc/").append(QString::number(PID)).append("/exe");
    QFileInfo info(filename);
    return info.readLink();
}

/*
STEApp* STEAppManager::getAppFromIdentifier(const QString& identifier)
{
    auto it = apps.find(identifier);

    if(it == apps.end())
        return nullptr;

    return it.value();
}
*/

STEApp* STEAppManager::getAppFromPID(quint64 PID)
{
    return nullptr;

    /*
    // TODO: Revice process identification method

    QString exe = getExecutablePath(PID);
    QFileInfo info(exe);
    QDir dir = info.absoluteDir();
    QString identifier = dir.dirName();

    return getAppFromIdentifier(identifier);
    */
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
