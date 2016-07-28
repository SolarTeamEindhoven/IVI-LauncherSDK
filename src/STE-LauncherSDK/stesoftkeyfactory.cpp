#include "stesoftkeyfactory.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/private/qfactoryloader_p.h>

#include <STE-LauncherSDK/STESoftKeyPlugin>

#include <QDebug>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader, (STESoftKeyPluginFactoryInterface_iid, QLatin1String("/ste-softkey"), Qt::CaseInsensitive))
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, directLoader, (STESoftKeyPluginFactoryInterface_iid, QLatin1String(""), Qt::CaseInsensitive))

QStringList STESoftKeyFactory::keys(const QString& pluginPath)
{
    qDebug() << "Plugin path:" << pluginPath;
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

STESoftKeyProvider* STESoftKeyFactory::create(const QString &name, const QStringList &args, const QString &pluginPath)
{
    if (!pluginPath.isEmpty())
    {
        QCoreApplication::addLibraryPath(pluginPath);
        if (STESoftKeyProvider* ret = qLoadPlugin<STESoftKeyProvider, STESoftKeyPlugin>(directLoader(), name, args))
            return ret;
    }
    if (STESoftKeyProvider* ret = qLoadPlugin<STESoftKeyProvider, STESoftKeyPlugin>(loader(), name, args))
        return ret;
}

QT_END_NAMESPACE
