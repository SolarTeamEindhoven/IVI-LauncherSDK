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
    Q_PROPERTY(QDBusVariant value READ getDbusValue NOTIFY valueChanged)

public:
    explicit STEVehicleData(QObject *parent = 0);

    virtual QString key() = 0;
    QDBusVariant getDbusValue();

Q_SIGNALS:
    void valueChanged(const QDBusVariant&);

protected:
    virtual QVariant getValue() = 0;
    void triggerValueChangedSignal();

private:
    VehicledataAdaptor* adaptor;
};

QT_END_NAMESPACE

#endif // STEVEHICLEDATA_H