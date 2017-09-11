#include "ivivehiclesetting.h"
#include "ivivehiclesetting_p.h"

#include <QtDBus/QDBusVariant>

QT_BEGIN_NAMESPACE

IVIVehicleSetting::IVIVehicleSetting(QObject* parent)
    : QObject(*new IVIVehicleSettingPrivate(this), parent)
{}

QVariant IVIVehicleSetting::getDbusValue() const {
    return QVariant::fromValue(QDBusVariant(getValue()));
}

IVIVehicleSettingPrivate::IVIVehicleSettingPrivate(IVIVehicleSetting* iviVehicleSetting)
    : adaptor(iviVehicleSetting)
{}

QT_END_NAMESPACE
