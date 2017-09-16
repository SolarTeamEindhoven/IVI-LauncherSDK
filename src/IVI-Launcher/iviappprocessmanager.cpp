#include "iviappprocessmanager_p.h"

#include <QDir>

#include "iviapplication.h"

QT_BEGIN_NAMESPACE

IVIAppProcessManager::IVIAppProcessManager(QObject* parent)
    : QObject(parent)
{
    env.insert("XDG_RUNTIME_DIR", qgetenv("XDG_RUNTIME_DIR"));
    env.insert("QT_QPA_PLATFORM", "wayland");
    env.insert("QT_WAYLAND_SHELL_INTEGRATION", "ivi-shell");
    env.insert("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1");
    env.insert("QT_WAYLAND_USE_BYPASSWINDOWMANAGERHINT", "1");
}

IVIAppProcessManager::~IVIAppProcessManager() {
    foreach(QProcess* process, processes) {
        process->kill();
        process->deleteLater();
    }
}

IVIAppProcessManager& IVIAppProcessManager::getInstance() {
    static IVIAppProcessManager appProcessManager;
    return appProcessManager;
}

qint64 IVIAppProcessManager::launch(IVIApplication& application) {
    auto process = new QProcess();
    process->setProcessEnvironment(env);
    process->setWorkingDirectory(application.getWorkingDirectory());
    process->setProgram(QDir(application.getWorkingDirectory()).absoluteFilePath(application.getExecutable()));
    process->setArguments(application.getArguments());

    connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &IVIAppProcessManager::handleProcessFinished);

    process->start(QProcess::NotOpen);

    processes.push_back(process);

    return process->processId();
}

void IVIAppProcessManager::handleProcessFinished(int /*exitCode*/) {
    QProcess* process = qobject_cast<QProcess*>(sender());
    if(process == nullptr)
        return;

    processes.removeOne(process);
    process->deleteLater();
}

QT_END_NAMESPACE
