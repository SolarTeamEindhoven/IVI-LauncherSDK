#ifndef STEDBUSMANAGER_H
#define STEDBUSMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "dbusmanager_adaptor.h"

class STEVehicleData;

class STEDBusManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList vehicle_data_keys READ getVehicleDataKeys NOTIFY vehicleDataKeysChanged)

public:
    explicit STEDBusManager(QObject* parent = 0);

    QStringList getVehicleDataKeys() const;

signals:
    void vehicleDataKeysChanged();

private:
    ManagerAdaptor managerAdaptor;
    QList<STEVehicleData*> vehicleDataList;

    void loadBackends();
};

#endif // STEDBUSMANAGER_H
