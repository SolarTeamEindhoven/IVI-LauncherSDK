#ifndef IVIAPPLICATION_P_H
#define IVIAPPLICATION_P_H

#include <QtCore/private/qobject_p.h>

#include "iviapplication.h"

QT_BEGIN_NAMESPACE

class QWaylandIviSurface;
class IVIManifest;

class IVIApplicationPrivate : public QObjectPrivate
{
public:
    explicit IVIApplicationPrivate(const IVIManifest& manifest, IVIApplication*);

    void addSurface(QWaylandIviSurface* iviSurface);

private:
    IVIApplication::RunningState runningState;
    QString name;
    QString description;
    QString icon;
    QString website;
    QList<QString> categories;
    QString executable;
    QList<QString> arguments;
    QString workingDirectory;

    QList<QWaylandIviSurface*> surfaces;

    void handleIviSurfaceDestroy(QObject*);
    void handleSurfaceDestroy();

    Q_DECLARE_PUBLIC(IVIApplication)
    Q_DISABLE_COPY(IVIApplicationPrivate)
};

QT_END_NAMESPACE

#endif // IVIAPPLICATION_P_H
