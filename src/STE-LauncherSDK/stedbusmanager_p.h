#ifndef STEDBUSMANAGER_H
#define STEDBUSMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "dbusmanager_adaptor.h"

class STEVehicleData;
class STEVehicleInteractionBackend;

class STEDBusManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList vehicle_data_keys READ getVehicleDataKeys NOTIFY vehicleDataKeysChanged)

public:
    explicit STEDBusManager(QObject* parent = 0);
    ~STEDBusManager();

    QStringList getVehicleDataKeys() const;

signals:
    void vehicleDataKeysChanged();

private:
    ManagerAdaptor managerAdaptor;
    QList<STEVehicleInteractionBackend*> backends;
    QList<STEVehicleData*> vehicleDataList;

    void loadBackends();
    void removeVehicleDataObject();
};

#endif // STEDBUSMANAGER_H
