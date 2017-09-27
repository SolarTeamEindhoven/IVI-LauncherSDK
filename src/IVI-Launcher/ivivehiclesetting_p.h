#ifndef IVIVEHICLESETTING_P_H
#define IVIVEHICLESETTING_P_H

#include <memory>

#include <QtCore/private/qobject_p.h>
#include <QtCore/QVariant>

#include <IVI-Launcher/ivilauncherglobal.h>
#include "dbusvehiclesetting_adaptor.h"

#include "ivivehiclesetting.h"

QT_BEGIN_NAMESPACE

class IVIVehicleSettingPrivate : public QObjectPrivate
{
public:
    IVIVehicleSettingPrivate();

    void initialize(IVIVehicleSetting*);

private:
    std::unique_ptr<VehiclesettingAdaptor> adaptor;

    Q_DECLARE_PUBLIC(IVIVehicleSetting)
    Q_DISABLE_COPY(IVIVehicleSettingPrivate)
};

QT_END_NAMESPACE

#endif // IVIVEHICLESETTING_P_H
