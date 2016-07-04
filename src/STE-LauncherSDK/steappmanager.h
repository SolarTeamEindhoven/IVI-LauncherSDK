#ifndef STEAPPMANAGER_H
#define STEAPPMANAGER_H

#include <QObject>
#include <QQmlListProperty>

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STEApp;
class QDir;

class Q_STE_LAUNCHERSDK_EXPORT STEAppManager: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<STEApp> appList READ getAppQMLList NOTIFY appListChanged)
//    Q_PROPERTY(QQmlListProperty<STEApp> appDirectories READ getAppDirectoriesList NOTIFY appDirectoriesListChanged)

public:
    STEAppManager(QObject* parent = nullptr);
    ~STEAppManager();

    void addKnownApp(const QString& identifier);
    const QList<STEApp*>& getAppList() const { return apps; }

    void addAppDirectory(const QString& directoryName);
    const QList<QDir>& getAppDirectories() { return appDirs; }

//    static STEApp* getAppFromIdentifier(const QString& identifier);
    static STEApp* getAppFromPID(quint64 PID);

signals:
    void appListChanged();
    void appDirectoriesListChanged();

private:
    static QList<STEApp*> apps;
    static QList<QDir> appDirs;

    QQmlListProperty<STEApp> getAppQMLList();
    static int QMLAppListCountFunction(QQmlListProperty<STEApp>* list);
    static STEApp* QMLAppListAtFunction(QQmlListProperty<STEApp>* list, int index);
};

QT_END_NAMESPACE

#endif // STEAPPMANAGER_H
