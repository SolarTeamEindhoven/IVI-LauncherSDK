#ifndef STEAPP_H
#define STEAPP_H

#include <QObject>
#include <QQmlListProperty>

#include "stemanifestparser.h"

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STEAppInstance;
class QDir;

class Q_STE_LAUNCHERSDK_EXPORT STEApp: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString workingDirectory READ getWorkingDirectory CONSTANT)
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QString description READ getDescription CONSTANT)
    Q_PROPERTY(QString icon READ getIcon CONSTANT)
    Q_PROPERTY(QString Website READ getWebsite CONSTANT)
    Q_PROPERTY(QQmlListProperty<const QString> categories READ getCategoriesQml CONSTANT)

public:
    STEApp(QDir& directory, QObject* parent = nullptr);
    ~STEApp();

    const QList<STEAppInstance*>& getInstances() const { return instances; }

    void launch();

    const QString& getWorkingDirectory() const {return workingDirectory;}
    const QString& getName() const {return manifest.getName();}
    const QString& getDescription() const {return manifest.getDescription();}
    const QString& getIcon() const {return manifest.getIcon();}
    const QString& getWebsite() const {return manifest.getWebsite();}
    const QList<QString>& getCategories() const {return manifest.getCategories();}
    const QString& getExecutable() const {return manifest.getExecutable();}
    const QList<QString>& getArguments() const {return manifest.getArguments();}

signals:
    void appInstanceAdded(STEAppInstance*);
    void appInstanceRemoved(STEAppInstance*);

private:
    QList<STEAppInstance*> instances;
    QString workingDirectory;
    STEManifest manifest;

    void addAppInstance(STEAppInstance* appInstance);
    void removeAppInstance(STEAppInstance* appInstance);
    void removeAppInstance();

    QQmlListProperty<const QString> getCategoriesQml() const;
    static int categoriesCountFunction(QQmlListProperty<const QString>*);
    static const QString* categoriesAtFunction(QQmlListProperty<const QString>*, int);

    friend class STEAppInstance;
};

QT_END_NAMESPACE

#endif // STEAPP_H
