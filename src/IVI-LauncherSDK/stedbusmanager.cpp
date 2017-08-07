#include "stedbusmanager_p.h"

#include <QtDBus/QDBusConnection>

#include <IVI-LauncherSDK/STEVehicleData>
#include <IVI-LauncherSDK/STEVehicleSetting>
#include <IVI-LauncherSDK/STEVehicleInteractionBackend>
#include <IVI-LauncherSDK/STEVehicleInteractionBackendFactory>

STEDBusManager::STEDBusManager(QObject* parent)
    : QObject(parent)
    , managerAdaptor(this)
{
    loadBackends();

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/Manager", this);
    connection.registerService("nl.solarteameindhoven.ivi");
}

STEDBusManager::~STEDBusManager()
{
    foreach(STEVehicleInteractionBackend* backend, backends)
    {
        delete backend;
    }
}

QStringList STEDBusManager::getVehicleDataKeys() const
{
    QStringList result;

    foreach(STEVehicleData* vehicleData, vehicleDataList)
        result.append(vehicleData->key());

    return result;
}

QStringList STEDBusManager::getVehicleSettingKeys() const
{
    QStringList result;

    foreach(STEVehicleSetting* vehicleSetting, vehicleSettingList)
        result.append(vehicleSetting->key());

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

            if(backend == nullptr)
                continue;

            backends.append(backend);
            {
                QList<STEVehicleData*> list = backend->createVehicleDataObjects();
                if(!list.empty())
                {
                    QDBusConnection connection = QDBusConnection::sessionBus();
                    foreach(STEVehicleData* vehicleData, list)
                    {
                        connect(vehicleData, &STEVehicleData::destroyed, this, &STEDBusManager::removeVehicleDataObject);
                        connection.registerObject("/VehicleData/" + vehicleData->key(), vehicleData);
                    }
                    vehicleDataList.append(list);
                    emit vehicleDataKeysChanged();
                }
            }
            {
                QList<STEVehicleSetting*> list = backend->createVehicleSettingObjects();
                if(!list.empty())
                {
                    QDBusConnection connection = QDBusConnection::sessionBus();
                    foreach(STEVehicleSetting* vehicleSetting, list)
                    {
                        connect(vehicleSetting, &STEVehicleSetting::destroyed, this, &STEDBusManager::removeVehicleSettingObject);
                        connection.registerObject("/VehicleSetting/" + vehicleSetting->key(), vehicleSetting);
                    }
                    vehicleSettingList.append(list);
                    emit vehicleSettingKeysChanged();
                }
            }
        }
    }
}

void STEDBusManager::removeVehicleDataObject()
{
    STEVehicleData* vehicleData = qobject_cast<STEVehicleData*>(sender());

    if(vehicleData != nullptr)
    {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterObject("/VehicleData/" + vehicleData->key());

        vehicleDataList.removeOne(vehicleData);
    }
}

void STEDBusManager::removeVehicleSettingObject()
{
    STEVehicleSetting* vehicleSetting = qobject_cast<STEVehicleSetting*>(sender());

    if(vehicleSetting != nullptr)
    {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterObject("/VehicleSetting/" + vehicleSetting->key());

        vehicleSettingList.removeOne(vehicleSetting);
    }
}
