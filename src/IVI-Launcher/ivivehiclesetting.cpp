#include "ivivehiclesetting_p.h"

#include <QtDBus/QDBusVariant>

QT_BEGIN_NAMESPACE

IVIVehicleSetting::IVIVehicleSetting(QObject* parent)
    : QObject(*new IVIVehicleSettingPrivate(), parent)
{
    Q_D(IVIVehicleSetting);
    d->initialize(this);
}

QVariant IVIVehicleSetting::getDbusValue() const {
    return QVariant::fromValue(QDBusVariant(getValue()));
}

IVIVehicleSettingPrivate::IVIVehicleSettingPrivate()
{}

void IVIVehicleSettingPrivate::initialize(IVIVehicleSetting* iviVehicleSetting) {
    adaptor = std::make_unique<VehiclesettingAdaptor>(iviVehicleSetting);
}

QT_END_NAMESPACE
