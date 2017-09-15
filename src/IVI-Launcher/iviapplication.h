#ifndef IVIAPPLICATION_H
#define IVIAPPLICATION_H

#include <QtCore/QObject>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class QWaylandClient;
class QWaylandIviSurface;
class IVIManifest;
class IVIApplicationPrivate;

class Q_IVI_LAUNCHER_EXPORT IVIApplication : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IVIApplication)

public:
    enum class RunningState {
        NotRunning,
        Starting,
        Running,
        Background
    };
    Q_ENUM(RunningState)

private:
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QString description READ getDescription CONSTANT)
    Q_PROPERTY(QString icon READ getIcon CONSTANT)
    Q_PROPERTY(QString website READ getWebsite CONSTANT)
    Q_PROPERTY(QString executable READ getExecutable CONSTANT)
    Q_PROPERTY(RunningState runningstate READ getRunningState NOTIFY runningStateChanged)

public:
    IVIApplication(const IVIManifest& manifest, QObject *parent = 0);

    const QList<QWaylandIviSurface*>& surfaces() const;

    const QString& getName() const;
    const QString& getDescription() const;
    const QString& getIcon() const;
    const QString& getWebsite() const;
    const QList<QString>& getCategories() const;
    const QString& getExecutable() const;
    const QList<QString>& getArguments() const;
    const QString& getWorkingDirectory() const;

    RunningState getRunningState() const;

    static IVIApplication* fromWaylandClient(QWaylandClient*);

signals:
    void newSurface(QWaylandIviSurface* iviSurface);
    void runningStateChanged();

public slots:
    void launch();

protected:
    IVIApplication(IVIApplicationPrivate& dd, QObject *parent = 0);
};

QT_END_NAMESPACE

#endif // IVIAPPLICATION_H
