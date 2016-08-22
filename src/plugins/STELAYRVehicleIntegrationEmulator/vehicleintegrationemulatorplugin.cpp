#include "vehicleintegrationemulatorplugin.h"

#include "vehicleinteractionemulatorbackend.h"

#include <QDebug>

VehicleIntegrationEmulatorPlugin::VehicleIntegrationEmulatorPlugin(QObject *parent)
    : STEVehicleInteractionBackendPlugin(parent)
{
}

STEVehicleInteractionBackend* VehicleIntegrationEmulatorPlugin::create(const QString &key, const QStringList &paramList)
{
    return new VehicleInteractionEmulatorBackend();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(STELAYRVehicleIntegrationEmulator, VehicleIntegrationEmulatorPlugin)
#endif // QT_VERSION < 0x050000
