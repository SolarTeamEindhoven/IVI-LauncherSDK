#include "ividbusmanager_p.h"

#include <QtDBus/QDBusConnection>

#include <IVI-Launcher/IVIVehicleData>
#include <IVI-Launcher/IVIVehicleSetting>
#include <IVI-Launcher/IVIVehicleInteractionBackend>
#include "ivivehicleinteractionbackendfactory_p.h"

IVIDBusManager::IVIDBusManager(QObject* parent)
    : QObject(parent)
    , managerAdaptor(this)
{
    loadBackends();

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/Manager", this);
    connection.registerService("nl.solarteameindhoven.ivi");
}

IVIDBusManager::~IVIDBusManager()
{
    foreach(IVIVehicleInteractionBackend* backend, backends)
    {
        delete backend;
    }
}

QStringList IVIDBusManager::getVehicleDataKeys() const
{
    QStringList result;

    foreach(IVIVehicleData* vehicleData, vehicleDataList)
        result.append(vehicleData->key());

    return result;
}

QStringList IVIDBusManager::getVehicleSettingKeys() const
{
    QStringList result;

    foreach(IVIVehicleSetting* vehicleSetting, vehicleSettingList)
        result.append(vehicleSetting->key());

    return result;
}

void IVIDBusManager::loadBackends()
{
    foreach(IVIVehicleData* vehicleData, vehicleDataList)
        vehicleData->deleteLater();
    vehicleDataList.clear();

    QStringList keys = IVIVehicleInteractionBackendFactory::keys();

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

            IVIVehicleInteractionBackend* backend = IVIVehicleInteractionBackendFactory::create(key, QStringList());

            if(backend == nullptr)
                continue;

            backends.append(backend);
            {
                QList<IVIVehicleData*> list = backend->createVehicleDataObjects();
                if(!list.empty())
                {
                    QDBusConnection connection = QDBusConnection::sessionBus();
                    foreach(IVIVehicleData* vehicleData, list)
                    {
                        connect(vehicleData, &IVIVehicleData::destroyed, this, &IVIDBusManager::removeVehicleDataObject);
                        connection.registerObject("/VehicleData/" + vehicleData->key(), vehicleData);
                    }
                    vehicleDataList.append(list);
                    emit vehicleDataKeysChanged();
                }
            }
            {
                QList<IVIVehicleSetting*> list = backend->createVehicleSettingObjects();
                if(!list.empty())
                {
                    QDBusConnection connection = QDBusConnection::sessionBus();
                    foreach(IVIVehicleSetting* vehicleSetting, list)
                    {
                        connect(vehicleSetting, &IVIVehicleSetting::destroyed, this, &IVIDBusManager::removeVehicleSettingObject);
                        connection.registerObject("/VehicleSetting/" + vehicleSetting->key(), vehicleSetting);
                    }
                    vehicleSettingList.append(list);
                    emit vehicleSettingKeysChanged();
                }
            }
        }
    }
}

void IVIDBusManager::removeVehicleDataObject()
{
    IVIVehicleData* vehicleData = qobject_cast<IVIVehicleData*>(sender());

    if(vehicleData != nullptr)
    {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterObject("/VehicleData/" + vehicleData->key());

        vehicleDataList.removeOne(vehicleData);
    }
}

void IVIDBusManager::removeVehicleSettingObject()
{
    IVIVehicleSetting* vehicleSetting = qobject_cast<IVIVehicleSetting*>(sender());

    if(vehicleSetting != nullptr)
    {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterObject("/VehicleSetting/" + vehicleSetting->key());

        vehicleSettingList.removeOne(vehicleSetting);
    }
}
