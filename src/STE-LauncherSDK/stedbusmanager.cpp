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
            QList<STEVehicleData*> list = backend->createVehicleDataObjects();
            if(!list.empty())
            {
                QDBusConnection connection = QDBusConnection::sessionBus();
                foreach(STEVehicleData* vehicleData, list)
                {
                    connect(vehicleData, &STEVehicleData::destroyed, this, &STEDBusManager::removeVehicleDataObject);
                    connection.registerObject("/VEHICLEDATA_" + vehicleData->key(), vehicleData);
                }
                vehicleDataList.append(list);
                emit vehicleDataKeysChanged();
            }
        }
    }
}

void STEDBusManager::removeVehicleDataObject()
{
    STEVehicleData* vehicleData = qobject_cast<STEVehicleData*>(sender());

    if(vehicleData != nullptr)
    {
        qDebug() << "Unregistring vehicle data object!";

        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterObject("/VEHICLEDATA_" + vehicleData->key());

        vehicleDataList.removeOne(vehicleData);
    }
}
