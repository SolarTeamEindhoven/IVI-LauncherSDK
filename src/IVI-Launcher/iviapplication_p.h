#ifndef IVIAPPLICATION_P_H
#define IVIAPPLICATION_P_H

#include <QtCore/private/qobject_p.h>

QT_BEGIN_NAMESPACE

class QWaylandIviSurface;
class IVIManifest;
class IVIApplication;

class IVIApplicationPrivate : public QObjectPrivate
{
public:
    explicit IVIApplicationPrivate(const IVIManifest& manifest, IVIApplication*);

    void addSurface(QWaylandIviSurface* iviSurface);

private:
    QString name;
    QString description;
    QString icon;
    QString website;
    QList<QString> categories;
    QString executable;
    QList<QString> arguments;

    QList<QWaylandIviSurface*> surfaces;

    void handleIviSurfaceDestroy(QObject*);
    void handleSurfaceDestroy();

    Q_DECLARE_PUBLIC(IVIApplication)
    Q_DISABLE_COPY(IVIApplicationPrivate)
};

QT_END_NAMESPACE

#endif // IVIAPPLICATION_P_H
