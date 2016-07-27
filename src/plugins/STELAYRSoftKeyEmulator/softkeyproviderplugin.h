#ifndef SOFTKEYPROVIDERPLUGIN_H
#define SOFTKEYPROVIDERPLUGIN_H

#include <STESoftKeyPlugin>

#include "layrsoftkeyemulatorprovider.h"

class SoftKeyProviderPlugin : public STESoftKeyPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID STESoftKeyPluginFactoryInterface_iid FILE "STELAYRSoftKeyEmulator.json")

public:
    SoftKeyProviderPlugin(QObject *parent = 0);

    STESoftKeyProvider* create(const QString&, const QStringList&) Q_DECL_OVERRIDE
    {
        return &provider;
    }

private:
    LAYRSoftKeyEmulatorProvider provider;
};

#endif // SOFTKEYPROVIDERPLUGIN_H
