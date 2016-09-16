#include "steappprocessmanager.h"

#include <QList>
#include <QProcess>
#include <QProcessEnvironment>

#include "steapp.h"

static QProcessEnvironment env;
static QList<QProcess*> processes;
//static QVector<QPair<quint64, >>

STEAppProcessManager::STEAppProcessManager(QObject* parent)
    : QObject(parent)
{
    env.insert("QT_PLUGIN_PATH", "/home/steven/STE/SDK/build-SDK-Desktop-Debug/plugins"); // TODO: Remove... or at least make dynamic
    env.insert("QT_QPA_PLATFORM", "wayland");
    env.insert("QT_WAYLAND_SHELL_INTEGRATION", "wayland-ste");
    env.insert("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1");
}

STEAppProcessManager::~STEAppProcessManager()
{
    ;
}

void STEAppProcessManager::launcheApp(STEApp* app)
{
    QProcess* process = new QProcess(app);

    if(process == nullptr)
        return;

    process->setProcessEnvironment(env);
    process->setWorkingDirectory("/home/steven/STE/SDK/build-TestApp-Desktop-Debug"); // TODO
    process->setProgram("TestApp"); // TODO

    process->start(QProcess::NotOpen);

    processes.append(process);
}
