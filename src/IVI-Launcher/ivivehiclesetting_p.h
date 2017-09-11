#ifndef IVIVEHICLESETTING_P_H
#define IVIVEHICLESETTING_P_H

#include <memory>

#include <QtCore/private/qobject_p.h>
#include <QtCore/QVariant>

#include <IVI-Launcher/ivilauncherglobal.h>
#include "dbusvehiclesetting_adaptor.h"

QT_BEGIN_NAMESPACE

class IVIVehicleSetting;

class IVIVehicleSettingPrivate : public QObjectPrivate
{
public:
    IVIVehicleSettingPrivate(IVIVehicleSetting*);

private:
    VehiclesettingAdaptor adaptor;

    Q_DECLARE_PUBLIC(IVIVehicleSetting)
    Q_DISABLE_COPY(IVIVehicleSettingPrivate)
};

QT_END_NAMESPACE

#endif // IVIVEHICLESETTING_P_H
