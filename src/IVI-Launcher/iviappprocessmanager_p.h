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
    qint64 launch(IVIApplication&);

    static IVIAppProcessManager& getInstance();

private:
    QProcessEnvironment env;
    QList<QProcess*> processes;

    explicit IVIAppProcessManager(QObject* parent = nullptr);
    ~IVIAppProcessManager();

    void handleProcessFinished(int);
};

QT_END_NAMESPACE

#endif // IVIAPPPROCESSMANAGER_H
