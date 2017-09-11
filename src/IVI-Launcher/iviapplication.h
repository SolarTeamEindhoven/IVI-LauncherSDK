#ifndef IVIAPPLICATION_H
#define IVIAPPLICATION_H

#include <QtCore/QObject>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class QWaylandClient;
class QWaylandIviSurface;
class IVIManifest;
class IVIApplicationPrivate;

class Q_IVI_LAUNCHER_EXPORT IVIApplication : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IVIApplication)
    Q_PROPERTY(QString name READ getName CONSTANT)

public:
    IVIApplication(const IVIManifest& manifest, QObject *parent = 0);

    const QList<QWaylandIviSurface*>& surfaces() const;
    const QString& getName() const;

    static IVIApplication* fromWaylandClient(QWaylandClient*);

signals:
    void newSurface(QWaylandIviSurface* iviSurface);

protected:
    IVIApplication(IVIApplicationPrivate& dd, QObject *parent = 0);
};

QT_END_NAMESPACE

#endif // IVIAPPLICATION_H
