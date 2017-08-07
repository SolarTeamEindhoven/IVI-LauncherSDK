#include "stevehicledata.h"

#include "dbusvehicledata_adaptor.h"

QT_BEGIN_NAMESPACE

STEVehicleData::STEVehicleData(QObject *parent)
    : QObject(parent)
    , adaptor(new VehicledataAdaptor(this))
{}

QVariant STEVehicleData::getDbusValue()
{
    return QVariant::fromValue(QDBusVariant(getValue()));
}

QT_END_NAMESPACE
