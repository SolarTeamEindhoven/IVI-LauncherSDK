#include "stevehicleinteractionbackendfactory.h"

#include <QtCore/private/qfactoryloader_p.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>

#include <STE-LauncherSDK/STEVehicleInteractionBackendPlugin>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader, (STEVehicleInteractionBackendPluginFactoryInterface_iid, QLatin1String("/ste-vehicleinteractionbackend"), Qt::CaseInsensitive))
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, directLoader, (STEVehicleInteractionBackendPluginFactoryInterface_iid, QLatin1String(""), Qt::CaseInsensitive))

QStringList STEVehicleInteractionBackendFactory::keys(const QString& pluginPath)
{
    QStringList list;
    if (!pluginPath.isEmpty())
    {
        QCoreApplication::addLibraryPath(pluginPath);
        list = directLoader()->keyMap().values();
        if (!list.isEmpty())
        {
            const QString postFix = QStringLiteral(" (from ")
                    + QDir::toNativeSeparators(pluginPath)
                    + QLatin1Char(')');
            const QStringList::iterator end = list.end();
            for (QStringList::iterator it = list.begin(); it != end; ++it)
                it->append(postFix);
        }
    }
    list.append(loader()->keyMap().values());
    return list;
}

STEVehicleInteractionBackend* STEVehicleInteractionBackendFactory::create(const QString &name, const QStringList &args, const QString &pluginPath)
{
    if (!pluginPath.isEmpty())
    {
        QCoreApplication::addLibraryPath(pluginPath);
        if (STEVehicleInteractionBackend* ret = qLoadPlugin<STEVehicleInteractionBackend, STEVehicleInteractionBackendPlugin>(directLoader(), name, args))
            return ret;
    }
    if (STEVehicleInteractionBackend* ret = qLoadPlugin<STEVehicleInteractionBackend, STEVehicleInteractionBackendPlugin>(loader(), name, args))
        return ret;

    return nullptr;
}

QT_END_NAMESPACE
