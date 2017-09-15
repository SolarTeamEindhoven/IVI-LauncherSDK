#ifndef IVIAPPPROCESSMANAGER_H
#define IVIAPPPROCESSMANAGER_H

#include <memory>

#include <QObject>
#include <QProcessEnvironment>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class IVIApplication;

class IVIAppProcessManager : public QObject
{
    Q_OBJECT

public:
    explicit IVIAppProcessManager(QObject* parent = nullptr);
    ~IVIAppProcessManager();

    qint64 launch(IVIApplication&);

private:
    QProcessEnvironment env;
    QList<QProcess*> processes;

    void handleProcessFinished(int);
};

QT_END_NAMESPACE

#endif // IVIAPPPROCESSMANAGER_H
