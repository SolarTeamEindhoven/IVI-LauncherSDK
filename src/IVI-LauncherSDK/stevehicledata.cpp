#include "stevehicledata.h"

#include "dbusvehicledata_adaptor.h"

QT_BEGIN_NAMESPACE

STEVehicleData::STEVehicleData(QObject *parent)
    : QObject(parent)
    , adaptor(new VehicledataAdaptor(this))
{
    connect(this, &STEVehicleData::valueChanged, adaptor, &VehicledataAdaptor::valueChanged);
}

QDBusVariant STEVehicleData::getDbusValue()
{
    return QDBusVariant(getValue());
}

void STEVehicleData::triggerValueChangedSignal()
{
    emit valueChanged(getDbusValue());
}

QT_END_NAMESPACE
