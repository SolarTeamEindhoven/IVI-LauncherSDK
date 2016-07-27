#ifndef STESOFTKEYFACTORY_H
#define STESOFTKEYFACTORY_H

#include <QStringList>

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STESoftKeyProvider;

class Q_STE_LAUNCHERSDK_EXPORT STESoftKeyFactory
{
public:
    static QStringList keys(const QString &pluginPath = QString());
    static STESoftKeyProvider* create(const QString &name, const QStringList &args, const QString &pluginPath = QString());
};

QT_END_NAMESPACE

#endif // STESOFTKEYFACTORY_H
