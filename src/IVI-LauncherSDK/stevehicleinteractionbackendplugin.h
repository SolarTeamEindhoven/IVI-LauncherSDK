#ifndef STEVEHICLEINTERACTIONBACKENDPLUGIN_H
#define STEVEHICLEINTERACTIONBACKENDPLUGIN_H

#include <QObject>

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STEVehicleInteractionBackend;

#define STEVehicleInteractionBackendPluginFactoryInterface_iid "nl.solarteameindhoven.launcher.STEVehicleInteractionBackendPluginFactoryInterface.1"

class Q_STE_LAUNCHERSDK_EXPORT STEVehicleInteractionBackendPlugin : public QObject
{
    Q_OBJECT

public:
    explicit STEVehicleInteractionBackendPlugin(QObject* parent = 0);
    virtual ~STEVehicleInteractionBackendPlugin();

    virtual STEVehicleInteractionBackend* create(const QString &key, const QStringList &paramList) = 0;
};

QT_END_NAMESPACE

#endif // STEVEHICLEINTERACTIONBACKENDPLUGIN_H
