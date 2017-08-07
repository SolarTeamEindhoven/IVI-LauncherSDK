#ifndef STEDBUSMANAGER_H
#define STEDBUSMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "dbusmanager_adaptor.h"

class STEVehicleData;
class STEVehicleSetting;
class STEVehicleInteractionBackend;

class STEDBusManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList vehicle_data_keys READ getVehicleDataKeys NOTIFY vehicleDataKeysChanged)
    Q_PROPERTY(QStringList vehicle_setting_keys READ getVehicleSettingKeys NOTIFY vehicleSettingKeysChanged)

public:
    explicit STEDBusManager(QObject* parent = 0);
    ~STEDBusManager();

    QStringList getVehicleDataKeys() const;
    QStringList getVehicleSettingKeys() const;

signals:
    void vehicleDataKeysChanged();
    void vehicleSettingKeysChanged();

private:
    ManagerAdaptor managerAdaptor;
    QList<STEVehicleInteractionBackend*> backends;
    QList<STEVehicleData*> vehicleDataList;
    QList<STEVehicleSetting*> vehicleSettingList;

    void loadBackends();
    void removeVehicleDataObject();
    void removeVehicleSettingObject();
};

#endif // STEDBUSMANAGER_H
