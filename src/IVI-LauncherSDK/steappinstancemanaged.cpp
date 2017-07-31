#include "steappinstancemanaged.h"

#include "steapp.h"

#include <QDir>
#include <QDebug>

STEAppInstanceManaged::STEAppInstanceManaged(STEApp* app, QObject* parent)
    : STEAppInstance(app, parent)
{
    QProcessEnvironment env;
    env.insert("XDG_RUNTIME_DIR", qgetenv("XDG_RUNTIME_DIR"));
    env.insert("QT_QPA_PLATFORM", "wayland");
    env.insert("QT_WAYLAND_SHELL_INTEGRATION", "wayland-ste");
    env.insert("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1");
    env.insert("QT_WAYLAND_USE_BYPASSWINDOWMANAGERHINT", "1");

    // TODO: Make dynamic
//    env.insert("QT_QUICK_CONTROLS_STYLE", "material");
//    env.insert("QT_QUICK_CONTROLS_MATERIAL_THEME", "Dark");
//    env.insert("QT_QUICK_CONTROLS_MATERIAL_ACCENT", "DeepOrange");
//    env.insert("QT_QUICK_CONTROLS_MATERIAL_PRIMARY", "Yellow");
//    env.insert("QT_QUICK_CONTROLS_MATERIAL_FOREGROUND", "Amber");
//    env.insert("QT_QUICK_CONTROLS_MATERIAL_BACKGROUND", "Amber");

    QDir dir(app->getWorkingDirectory());

    process.setProcessEnvironment(env);
    process.setWorkingDirectory(app->getWorkingDirectory());
    process.setProgram(dir.absoluteFilePath(app->getExecutable()));
    qDebug() << "Running" << dir.absoluteFilePath(app->getExecutable()) << "with arguments:" << app->getArguments();
    process.setArguments(app->getArguments());

    connect(&process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &STEAppInstanceManaged::processFinished);
    connect(&process, &QProcess::readyReadStandardOutput, this, &STEAppInstanceManaged::readyRead);
    connect(&process, &QProcess::readyReadStandardError, this, &STEAppInstanceManaged::readyReadError);

//    process.start(QProcess::NotOpen);
    process.start(QProcess::ReadOnly);

    qDebug() << "Launched app as process:" << process.processId();
}


void STEAppInstanceManaged::processFinished(int exitCode)
{
    qDebug() << "Process" << process.processId() << "( app" << getApp()->getName() << ") exited!";
}

void STEAppInstanceManaged::readyRead()
{
    qDebug() << "Process" << process.processId() << "reports:" << process.readAllStandardOutput();
}

void STEAppInstanceManaged::readyReadError()
{
    qDebug() << "Process" << process.processId() << "reports:" << process.readAllStandardError();
}
