#include "stesoftkeymanager.h"

#include <STE-LauncherSDK/STESoftKeyFactory>
#include <STE-LauncherSDK/STESoftKeyProvider>
#include <STE-LauncherSDK/STESoftKey>
#include <STE-LauncherSDK/STEAppInstance>

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
    qDebug() << "-- Loading softkey provider using plugin keys:" << keys;
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
