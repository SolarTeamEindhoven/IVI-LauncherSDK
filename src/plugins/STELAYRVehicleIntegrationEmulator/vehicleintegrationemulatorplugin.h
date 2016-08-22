#ifndef VEHICLEINTEGRATIONEMULATORPLUGIN_H
#define VEHICLEINTEGRATIONEMULATORPLUGIN_H

#include <STEVehicleInteractionBackendPlugin>

class VehicleIntegrationEmulatorPlugin : public STEVehicleInteractionBackendPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID STEVehicleInteractionBackendPluginFactoryInterface_iid FILE "STELAYRVehicleIntegrationEmulator.json")
#endif // QT_VERSION >= 0x050000

public:
    VehicleIntegrationEmulatorPlugin(QObject *parent = 0);

    STEVehicleInteractionBackend* create(const QString &key, const QStringList &paramList) Q_DECL_OVERRIDE;
};

#endif // VEHICLEINTEGRATIONEMULATORPLUGIN_H
