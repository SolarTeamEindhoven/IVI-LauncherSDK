#include "ivivehicledata_p.h"

QT_BEGIN_NAMESPACE

IVIVehicleData::IVIVehicleData(QObject *parent)
    : QObject(*new IVIVehicleDataPrivate(), parent)
{
    Q_D(IVIVehicleData);
    d->initialize(this);
}

QVariant IVIVehicleData::getDbusValue()
{
    return QVariant::fromValue(QDBusVariant(getValue()));
}

IVIVehicleDataPrivate::IVIVehicleDataPrivate()
{}

void IVIVehicleDataPrivate::initialize(IVIVehicleData* iviVehicleData) {
    adaptor = std::make_unique<VehicledataAdaptor>(iviVehicleData);
}

QT_END_NAMESPACE
