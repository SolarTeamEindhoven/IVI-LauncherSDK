#include "vehicledata.h"

VehicleData::VehicleData(QObject *parent)
    : STEVehicleData(parent)
{

}

QString VehicleData::key()
{
    return name;
}

QVariant VehicleData::getValue()
{
    return value;
}
