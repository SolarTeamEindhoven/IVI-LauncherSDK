#include "stevehicleinteractionbackend.h"

QT_BEGIN_NAMESPACE

STEVehicleInteractionBackend::STEVehicleInteractionBackend(QObject* parent)
    : QObject(parent)
{}

QList<STEVehicleData*> STEVehicleInteractionBackend::createVehicleDataObjects() {
    return QList<STEVehicleData*>{};
}

QList<STEVehicleSetting*> STEVehicleInteractionBackend::createVehicleSettingObjects() {
    return QList<STEVehicleSetting*>{};
}

QT_END_NAMESPACE
