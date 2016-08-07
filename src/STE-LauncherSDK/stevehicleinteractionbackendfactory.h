#ifndef STEVEHICLEDATAFACTORY_H
#define STEVEHICLEDATAFACTORY_H

#include <QObject>

QT_BEGIN_NAMESPACE

class STEVehicleInteractionBackend;

class STEVehicleInteractionBackendFactory
{
public:
    static QStringList keys(const QString &pluginPath = QString());
    static STEVehicleInteractionBackend* create(const QString &name, const QStringList &args, const QString &pluginPath = QString());
};

QT_END_NAMESPACE

#endif // STEVEHICLEDATAFACTORY_H
