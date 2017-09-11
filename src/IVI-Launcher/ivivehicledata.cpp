#include "ivivehicledata_p.h"

QT_BEGIN_NAMESPACE

IVIVehicleData::IVIVehicleData(QObject *parent)
    : QObject(*new IVIVehicleDataPrivate(this), parent)
{}

QVariant IVIVehicleData::getDbusValue()
{
    return QVariant::fromValue(QDBusVariant(getValue()));
}

IVIVehicleDataPrivate::IVIVehicleDataPrivate(IVIVehicleData* iviVehicleData)
    : adaptor(iviVehicleData)
{}

QT_END_NAMESPACE
