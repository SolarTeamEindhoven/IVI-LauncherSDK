#ifndef STEVEHICLEINTERACTIONBACKEND_H
#define STEVEHICLEINTERACTIONBACKEND_H

#include <QObject>

class STEVehicleData;

class STEVehicleInteractionBackend : public QObject
{
    Q_OBJECT

public:
    explicit STEVehicleInteractionBackend(QObject* parent = nullptr);

    virtual QList<STEVehicleData*> createVehicleDataObjects() = 0;
};

#endif // STEVEHICLEINTERACTIONBACKEND_H
