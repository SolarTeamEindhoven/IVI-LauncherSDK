#ifndef IVIVEHICLEINTERACTIONBACKEND_H
#define IVIVEHICLEINTERACTIONBACKEND_H

#include <QObject>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class IVIVehicleData;
class IVIVehicleSetting;

class Q_IVI_LAUNCHER_EXPORT IVIVehicleInteractionBackend : public QObject
{
    Q_OBJECT

public:
    explicit IVIVehicleInteractionBackend(QObject* parent = nullptr);

    virtual QList<IVIVehicleData*> createVehicleDataObjects();
    virtual QList<IVIVehicleSetting*> createVehicleSettingObjects();
};

QT_END_NAMESPACE

#endif // IVIVEHICLEINTERACTIONBACKEND_H
