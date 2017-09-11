#ifndef IVIVEHICLEDATA_H
#define IVIVEHICLEDATA_H

#include <QtCore/QObject>
#include <QtDBus/QDBusVariant>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class VehicledataAdaptor;
class IVIVehicleDataPrivate;

class Q_IVI_LAUNCHER_EXPORT IVIVehicleData : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IVIVehicleData)
    Q_PROPERTY(QVariant value READ getDbusValue NOTIFY valueChanged)

public:
    explicit IVIVehicleData(QObject *parent = 0);

    virtual QString key() = 0;
    virtual QVariant getValue() = 0;

Q_SIGNALS:
    void valueChanged();

private:
    QVariant getDbusValue();
};

QT_END_NAMESPACE

#endif // IVIVEHICLEDATA_H
