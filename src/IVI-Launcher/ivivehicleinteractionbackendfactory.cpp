#include "ivivehicleinteractionbackendfactory_p.h"

#include <QtCore/private/qfactoryloader_p.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>

#include <IVI-Launcher/IVIVehicleInteractionBackendPlugin>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader, (IVIVehicleInteractionBackendPluginFactoryInterface_iid, QLatin1String("/ivi-vehicleinteractionbackend"), Qt::CaseInsensitive))
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, directLoader, (IVIVehicleInteractionBackendPluginFactoryInterface_iid, QLatin1String(""), Qt::CaseInsensitive))

QStringList IVIVehicleInteractionBackendFactory::keys(const QString& pluginPath)
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

IVIVehicleInteractionBackend* IVIVehicleInteractionBackendFactory::create(const QString& name, const QStringList& args, const QString& pluginPath)
{
    if (!pluginPath.isEmpty())
    {
        QCoreApplication::addLibraryPath(pluginPath);
        if (IVIVehicleInteractionBackend* ret = qLoadPlugin<IVIVehicleInteractionBackend, IVIVehicleInteractionBackendPlugin>(directLoader(), name, args))
            return ret;
    }
    if (IVIVehicleInteractionBackend* ret = qLoadPlugin<IVIVehicleInteractionBackend, IVIVehicleInteractionBackendPlugin>(loader(), name, args))
        return ret;

    return nullptr;
}

QT_END_NAMESPACE
