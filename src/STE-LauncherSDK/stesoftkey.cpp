#include "stesoftkey.h"

#include <STE-LauncherSDK/STESoftKeyManager>

STESoftKey::STESoftKey(QObject* parent)
    : QObject(parent)
    , ID(STESoftKeyManager::instance()->getUniqueID())
{
}

STESoftKey::~STESoftKey()
{
}
