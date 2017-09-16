/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt OTA Update module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <IVI-Launcher/IVIAppContainer>
//#include <IVI-Launcher/private/ivilauncherqml_p.h>
#include <IVI-Launcher/IVIApplication>
#include <IVI-Launcher/IVIApplicationManager>
#include <QtQml>

#include "iviapplicationmanagerqml.h"
#include "iviappcontainerqml.h"

QT_BEGIN_NAMESPACE

class IVILauncherPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("ivi.launcher"));

        qmlRegisterType<IVIAppContainerQml>(uri, 1, 0, "AppContainer");
        qmlRegisterUncreatableType<IVIApplication>(uri, 1, 0, "Application", "Applications can be created only using an application manager");
        qmlRegisterType<IVIApplicationManagerQML>(uri, 1, 0, "ApplicationManager");
    }
};

QT_END_NAMESPACE

#include "pluginmain.moc"
