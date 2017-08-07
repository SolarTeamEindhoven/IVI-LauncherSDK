#include "stevehiclesetting.h"

#include <QtDBus/QDBusVariant>
#include "dbusvehiclesetting_adaptor.h"

QT_BEGIN_NAMESPACE

STEVehicleSetting::STEVehicleSetting(QObject* parent)
    : QObject(parent)
    , adaptor(new VehiclesettingAdaptor(this))
{}

STEVehicleSetting::~STEVehicleSetting() {
    if(adaptor != nullptr)
        delete adaptor;
}

QVariant STEVehicleSetting::getDbusValue() const {
    return QVariant::fromValue(QDBusVariant(getValue()));
}

QT_END_NAMESPACE
