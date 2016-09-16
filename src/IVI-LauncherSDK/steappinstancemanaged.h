#ifndef STEAPPINSTANCEMANAGED_H
#define STEAPPINSTANCEMANAGED_H

#include "steappinstance.h"

#include <QProcess>

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STEApp;

class Q_STE_LAUNCHERSDK_EXPORT STEAppInstanceManaged : public STEAppInstance
{
public:
    STEAppInstanceManaged(STEApp* app, QObject* parent = nullptr);

    quint64 getPID() const Q_DECL_OVERRIDE { return process.processId(); }

private slots:
    void processFinished(int exitCode);
    void readyRead();
    void readyReadError();

private:
    QProcess process;
};

QT_END_NAMESPACE

#endif // STEAPPINSTANCEMANAGED_H
