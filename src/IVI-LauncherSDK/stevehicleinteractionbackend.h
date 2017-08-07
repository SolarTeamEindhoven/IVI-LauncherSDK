#ifndef STEVEHICLEINTERACTIONBACKEND_H
#define STEVEHICLEINTERACTIONBACKEND_H

#include <QObject>

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STEVehicleData;
class STEVehicleSetting;

class Q_STE_LAUNCHERSDK_EXPORT STEVehicleInteractionBackend : public QObject
{
    Q_OBJECT

public:
    explicit STEVehicleInteractionBackend(QObject* parent = nullptr);

    virtual QList<STEVehicleData*> createVehicleDataObjects();
    virtual QList<STEVehicleSetting*> createVehicleSettingObjects();
};

QT_END_NAMESPACE

#endif // STEVEHICLEINTERACTIONBACKEND_H
