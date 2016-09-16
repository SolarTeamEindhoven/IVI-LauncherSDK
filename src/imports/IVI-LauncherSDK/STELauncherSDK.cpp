#include <QtQml/QQmlExtensionPlugin>

#include <IVI-LauncherSDK/STEAppContainer>
#include <IVI-LauncherSDK/STEApp>
#include <IVI-LauncherSDK/STEAppManager>

class STELauncherSDKModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("solarteameindhoven.launcher"));

        qmlRegisterType<STEAppContainer>(uri, 1, 0, "AppContainer");
        qmlRegisterUncreatableType<STEApp>(uri, 1, 0, "App", "Apps can only be loaded using the AppManager");
        qmlRegisterType<STEAppManager>(uri, 1, 0, "AppManager");
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};

#include "STELauncherSDK.moc"
