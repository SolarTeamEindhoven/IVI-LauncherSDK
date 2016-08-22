#ifndef VEHICLEDATA_H
#define VEHICLEDATA_H

#include <STEVehicleData>

class VehicleData : public STEVehicleData
{
    Q_OBJECT
public:
    explicit VehicleData(QObject *parent = 0);

    QString key() Q_DECL_OVERRIDE;

    QString name;
    QVariant value;

protected:
    QVariant getValue() Q_DECL_OVERRIDE;
};

#endif // VEHICLEDATA_H
