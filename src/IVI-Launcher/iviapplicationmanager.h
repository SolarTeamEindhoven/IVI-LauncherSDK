#ifndef IVIAPPLICATIONMANAGER_H
#define IVIAPPLICATIONMANAGER_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class IVIApplication;

class Q_IVI_LAUNCHER_EXPORT IVIApplicationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<IVIApplication> applications READ getQmlApplicationList NOTIFY applicationListChanged)

public:
    explicit IVIApplicationManager(QObject* parent = nullptr);
    ~IVIApplicationManager();

    QQmlListProperty<IVIApplication> getQmlApplicationList() const;

    static void registerApplication(IVIApplication*); // TODO: Hide from user

signals:
    void applicationListChanged();
};

QT_END_NAMESPACE

#endif // IVIAPPLICATIONMANAGER_H
