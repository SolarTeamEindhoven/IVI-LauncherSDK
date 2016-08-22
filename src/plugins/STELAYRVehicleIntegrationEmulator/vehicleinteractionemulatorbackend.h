#ifndef VEHICLEINTERACTIONEMULATORBACKEND_H
#define VEHICLEINTERACTIONEMULATORBACKEND_H

#include <QMainWindow>
#include <QGridLayout>
#include <QFile>

#include <STEVehicleInteractionBackend>

class VehicleDataField;

class VehicleInteractionEmulatorBackend : public STEVehicleInteractionBackend
{
    Q_OBJECT
public:
    explicit VehicleInteractionEmulatorBackend(QObject *parent = 0);
    ~VehicleInteractionEmulatorBackend();

    QList<STEVehicleData*> createVehicleDataObjects() Q_DECL_OVERRIDE;

private:
    QMainWindow window;
    QWidget centerWidget;
    QGridLayout layout;
    QFile file;

    QList<VehicleDataField*> vehicleDataFields;

private slots:
    void addDataField();
};

#endif // VEHICLEINTERACTIONEMULATORBACKEND_H
