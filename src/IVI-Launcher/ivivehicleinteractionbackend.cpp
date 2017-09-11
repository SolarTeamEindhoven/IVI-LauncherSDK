#include "ivivehicleinteractionbackend.h"

QT_BEGIN_NAMESPACE

IVIVehicleInteractionBackend::IVIVehicleInteractionBackend(QObject* parent)
    : QObject(parent)
{}

QList<IVIVehicleData*> IVIVehicleInteractionBackend::createVehicleDataObjects() {
    return QList<IVIVehicleData*>{};
}

QList<IVIVehicleSetting*> IVIVehicleInteractionBackend::createVehicleSettingObjects() {
    return QList<IVIVehicleSetting*>{};
}

QT_END_NAMESPACE
