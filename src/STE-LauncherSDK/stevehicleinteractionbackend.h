#ifndef STEVEHICLEINTERACTIONBACKEND_H
#define STEVEHICLEINTERACTIONBACKEND_H

#include <QObject>

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STEVehicleData;

class Q_STE_LAUNCHERSDK_EXPORT STEVehicleInteractionBackend : public QObject
{
    Q_OBJECT

public:
    explicit STEVehicleInteractionBackend(QObject* parent = nullptr);

    virtual QList<STEVehicleData*> createVehicleDataObjects() = 0;
};

QT_END_NAMESPACE

#endif // STEVEHICLEINTERACTIONBACKEND_H
