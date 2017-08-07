#ifndef STEVEHICLEDATA_H
#define STEVEHICLEDATA_H

#include <QObject>

#include <QtDBus/QDBusVariant>

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class VehicledataAdaptor;

class Q_STE_LAUNCHERSDK_EXPORT STEVehicleData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ getDbusValue NOTIFY valueChanged)

public:
    explicit STEVehicleData(QObject *parent = 0);

    virtual QString key() = 0;
    virtual QVariant getValue() = 0;

Q_SIGNALS:
    void valueChanged();

private:
    VehicledataAdaptor* adaptor;

    QVariant getDbusValue();
};

QT_END_NAMESPACE

#endif // STEVEHICLEDATA_H
