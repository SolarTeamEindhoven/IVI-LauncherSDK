#ifndef IVIVEHICLEDATA_P_H
#define IVIVEHICLEDATA_P_H

#include <memory>

#include <QtCore/private/qobject_p.h>

#include <QtDBus/QDBusVariant>

#include "dbusvehicledata_adaptor.h"
#include "ivivehicledata.h"

QT_BEGIN_NAMESPACE

class IVIVehicleDataPrivate : public QObjectPrivate
{
public:
    explicit IVIVehicleDataPrivate();
    void initialize(IVIVehicleData*);

private:
    std::unique_ptr<VehicledataAdaptor> adaptor;

    Q_DECLARE_PUBLIC(IVIVehicleData)
    Q_DISABLE_COPY(IVIVehicleDataPrivate)
};

QT_END_NAMESPACE

#endif // IVIVEHICLEDATA_P_H
