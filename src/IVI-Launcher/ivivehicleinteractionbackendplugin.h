#ifndef IVIVEHICLEINTERACTIONBACKENDPLUGIN_H
#define IVIVEHICLEINTERACTIONBACKENDPLUGIN_H

#include <QObject>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class IVIVehicleInteractionBackend;

#define IVIVehicleInteractionBackendPluginFactoryInterface_iid "ivi.launcher.IVIVehicleInteractionBackendPluginInterface.1"

class Q_IVI_LAUNCHER_EXPORT IVIVehicleInteractionBackendPlugin : public QObject
{
    Q_OBJECT

public:
    explicit IVIVehicleInteractionBackendPlugin(QObject* parent = 0);
    virtual ~IVIVehicleInteractionBackendPlugin();

    virtual IVIVehicleInteractionBackend* create(const QString& key, const QStringList& paramList) = 0;
};

QT_END_NAMESPACE

#endif // IVIVEHICLEINTERACTIONBACKENDPLUGIN_H
