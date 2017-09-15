#ifndef IVIAPPLICATIONMANAGERQML_H
#define IVIAPPLICATIONMANAGERQML_H

#include <QObject>
#include <QQmlListProperty>

#include <IVIApplication>

class IVIApplicationManagerQML : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<IVIApplication> applications READ getApplicationList NOTIFY applicationListChanged)

public:
    explicit IVIApplicationManagerQML(QObject *parent = 0);

signals:
    void applicationListChanged();

private:
    QQmlListProperty<IVIApplication> getApplicationList() const;
};

#endif // IVIAPPLICATIONMANAGERQML_H
