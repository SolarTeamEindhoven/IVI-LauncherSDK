#ifndef IVIAPPLICATIONMANAGER_P_H
#define IVIAPPLICATIONMANAGER_P_H

#include <QtCore/private/qobject_p.h>
#include <QtQml/QQmlListProperty>

#include "iviapplicationmanager.h"

QT_BEGIN_NAMESPACE

class IVIApplicationManagerPrivate : public QObjectPrivate
{
public:
    IVIApplicationManagerPrivate();
    ~IVIApplicationManagerPrivate();

    void initialize();

    static void registerApplication(IVIApplication*);
    static Q_IVI_LAUNCHER_EXPORT const QList<IVIApplication*>& getApplications() noexcept;

signals:
    void applicationListChanged();

private:
    QList<IVIApplication*> applications;

    void handleApplicationRegistration(IVIApplication*);

    Q_DECLARE_PUBLIC(IVIApplicationManager)
    Q_DISABLE_COPY(IVIApplicationManagerPrivate)
};

QT_END_NAMESPACE

#endif // IVIAPPLICATIONMANAGER_P_H
