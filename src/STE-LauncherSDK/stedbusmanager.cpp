#include "stedbusmanager_p.h"

#include <QtDBus/QDBusConnection>

#include <STE-LauncherSDK/STEVehicleData>
#include <STE-LauncherSDK/STEVehicleInteractionBackend>
#include <STE-LauncherSDK/STEVehicleInteractionBackendFactory>

STEDBusManager::STEDBusManager(QObject* parent)
    : QObject(parent)
    , managerAdaptor(this)
{
    loadBackends();

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/Manager", this);
    connection.registerService("nl.solarteameindhoven.ivi");
}

QStringList STEDBusManager::getVehicleDataKeys() const
{
    QStringList result;

    foreach(STEVehicleData* vehicleData, vehicleDataList)
        result.append(vehicleData->key());

    return result;
}

void STEDBusManager::loadBackends()
{
    foreach(STEVehicleData* vehicleData, vehicleDataList)
        vehicleData->deleteLater();
    vehicleDataList.clear();

    QStringList keys = STEVehicleInteractionBackendFactory::keys();

    if (!keys.isEmpty()) {
        QStringList requestedKeys;
        if(qEnvironmentVariableIsSet("STE_VEHICLE_INTERACTION_BACKENDS"))
        {
            QString requestedKeyList = QString::fromUtf8(qgetenv("STE_VEHICLE_INTERACTION_BACKENDS"));
            requestedKeys = requestedKeyList.split(':');
        }
        else
        {
            requestedKeys = keys;
        }

        foreach(QString key, requestedKeys)
        {
            if(!keys.contains(key))
                continue;

            STEVehicleInteractionBackend* backend = STEVehicleInteractionBackendFactory::create(key, QStringList());
            QList<STEVehicleData*> list = backend->createVehicleDataObjects();
            if(!list.empty())
            {
                vehicleDataList.append(list);
                emit vehicleDataKeysChanged();
            }
        }
    }
}
