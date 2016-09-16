#include "stesoftkey.h"

#include <IVI-LauncherSDK/STESoftKeyManager>

STESoftKey::STESoftKey(QObject* parent)
    : QObject(parent)
    , ID(STESoftKeyManager::instance()->getUniqueID())
{
}

STESoftKey::~STESoftKey()
{
}
