#ifndef STEDBUSMANAGER_H
#define STEDBUSMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "dbusmanager_adaptor.h"

class IVIVehicleData;
class IVIVehicleSetting;
class IVIVehicleInteractionBackend;

class IVIDBusManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList vehicle_data_keys READ getVehicleDataKeys NOTIFY vehicleDataKeysChanged)
    Q_PROPERTY(QStringList vehicle_setting_keys READ getVehicleSettingKeys NOTIFY vehicleSettingKeysChanged)

public:
    explicit IVIDBusManager(QObject* parent = 0);
    ~IVIDBusManager();

    QStringList getVehicleDataKeys() const;
    QStringList getVehicleSettingKeys() const;

signals:
    void vehicleDataKeysChanged();
    void vehicleSettingKeysChanged();

private:
    ManagerAdaptor managerAdaptor;
    QList<IVIVehicleInteractionBackend*> backends;
    QList<IVIVehicleData*> vehicleDataList;
    QList<IVIVehicleSetting*> vehicleSettingList;

    void loadBackends();
    void removeVehicleDataObject();
    void removeVehicleSettingObject();
};

#endif // STEDBUSMANAGER_H
