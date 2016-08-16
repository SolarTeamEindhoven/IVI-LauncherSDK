#ifndef STESOFTKEYBACKENDPLUGIN_H
#define STESOFTKEYBACKENDPLUGIN_H

#include <QObject>
#include <QList>

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STESoftKeyProvider;

#define STESoftKeyPluginFactoryInterface_iid "nl.solarteameindhoven.launcher.STESoftKeyPluginFactoryInterface.1"

class Q_STE_LAUNCHERSDK_EXPORT STESoftKeyPlugin : public QObject
{
    Q_OBJECT

public:
    explicit STESoftKeyPlugin(QObject* parent = nullptr);
    virtual ~STESoftKeyPlugin();

    virtual STESoftKeyProvider* create(const QString &key, const QStringList &paramList) = 0;
};

QT_END_NAMESPACE

#endif // STESOFTKEYBACKENDPLUGIN_H
