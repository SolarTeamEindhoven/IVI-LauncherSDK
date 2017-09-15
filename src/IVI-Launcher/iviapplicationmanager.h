#ifndef IVIAPPLICATIONMANAGER_H
#define IVIAPPLICATIONMANAGER_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class IVIApplication;
class IVIApplicationManagerPrivate;

class Q_IVI_LAUNCHER_EXPORT IVIApplicationManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IVIApplicationManager)

public:
    QList<IVIApplication*> getApplicationList() const;

    static IVIApplicationManager& getInstance();

signals:
    void applicationListChanged();

private:
    explicit IVIApplicationManager(QObject* parent = nullptr);
    ~IVIApplicationManager();
};

QT_END_NAMESPACE

#endif // IVIAPPLICATIONMANAGER_H
