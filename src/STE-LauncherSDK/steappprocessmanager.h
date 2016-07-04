#ifndef STEAPPPROCESSMANAGER_H
#define STEAPPPROCESSMANAGER_H

#include <QObject>

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class QProcess;
class QProcessEnvironment;
class STEApp;

class Q_STE_LAUNCHERSDK_EXPORT STEAppProcessManager: public QObject
{
    Q_OBJECT

public:
    STEAppProcessManager(QObject* parent = nullptr);
    ~STEAppProcessManager();

    static void launcheApp(STEApp* app);
};

QT_END_NAMESPACE

#endif // STEAPPPROCESSMANAGER_H
