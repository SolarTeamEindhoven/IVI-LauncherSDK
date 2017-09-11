#ifndef IVIVEHICLEDATAFACTORY_H
#define IVIVEHICLEDATAFACTORY_H

#include <QObject>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class IVIVehicleInteractionBackend;

class Q_IVI_LAUNCHER_EXPORT IVIVehicleInteractionBackendFactory
{
public:
    static QStringList keys(const QString& pluginPath = QString());
    static IVIVehicleInteractionBackend* create(const QString& name, const QStringList& args, const QString& pluginPath = QString());
};

QT_END_NAMESPACE

#endif // IVIVEHICLEDATAFACTORY_H
