#include "stesoftkeymanager.h"

#include <IVI-LauncherSDK/STESoftKeyFactory>
#include <IVI-LauncherSDK/STESoftKeyProvider>
#include <IVI-LauncherSDK/STESoftKey>
#include <IVI-LauncherSDK/STEAppInstance>

#include <QQuickItem>

static STESoftKeyManager* managerInstance = nullptr;

STESoftKeyManager::STESoftKeyManager(QObject *parent)
    : QObject(parent)
    , visualizer()
    , pluginsLoaded(false)
    , maxID(0)
{
    visualizer.setFlag(QQuickItem::ItemHasContents, true);
}

const QList<STESoftKey*>& STESoftKeyManager::getSoftkeyList()
{
    if(pluginsLoaded == false)
        loadProviders();

    return softkeys;
}

STESoftKey* STESoftKeyManager::fromID(uint32_t ID)
{
    foreach(STESoftKey* softkey, softkeys)
    {
        if(softkey->getID() == ID)
            return softkey;
    }

    return nullptr;
}

uint32_t STESoftKeyManager::getUniqueID()
{
    return maxID++;
}

QQuickItem* STESoftKeyManager::item()
{
    return &visualizer;
}

STESoftKeyManager* STESoftKeyManager::instance()
{
    if(managerInstance == nullptr)
        managerInstance = new STESoftKeyManager;

    return managerInstance;
}

void STESoftKeyManager::changeActiveApp(STEAppInstance* appInstance)
{
    foreach(STESoftKeyProvider* provider, providers)
    {
        provider->activeAppChanged(appInstance);
    }
    // TODO: Inform launcher of active app change
}

void STESoftKeyManager::loadProviders()
{
    QStringList keys = STESoftKeyFactory::keys();

    if (!keys.isEmpty()) {
        QStringList requestedKeys;
        if(qEnvironmentVariableIsSet("STE_SOFTKEY_PROVIDERS"))
        {
            QString requestedKeyList = QString::fromUtf8(qgetenv("STE_SOFTKEY_PROVIDERS"));
            requestedKeys = requestedKeyList.split(':');
        }
        else
        {
            requestedKeys = keys;
        }

        foreach(QString key, requestedKeys)
        {
            if(!keys.contains(key))
                continue;

            STESoftKeyProvider* provider = STESoftKeyFactory::create(key, QStringList());

            if(provider == nullptr)
            {
                qWarning() << "Plugin" << key << "returned null pointer as provider!";
                continue;
            }

            providers.append(provider);
            connect(provider, &STESoftKeyProvider::changeActiveApp, this, &STESoftKeyManager::changeActiveApp);
            softkeys.append(provider->createSoftKeys());
            QQuickItem* vis = provider->createSoftKeyVisualization();
            if(vis != nullptr)
                vis->setParentItem(&visualizer);
        }
    }
    pluginsLoaded = true;
}
