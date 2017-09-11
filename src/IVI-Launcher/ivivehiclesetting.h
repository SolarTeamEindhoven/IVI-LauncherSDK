#ifndef STEVEHICLESETTING_H
#define STEVEHICLESETTING_H

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class VehiclesettingAdaptor;
class IVIVehicleSettingPrivate;

class Q_IVI_LAUNCHER_EXPORT IVIVehicleSetting : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IVIVehicleSetting)
    Q_PROPERTY(QVariant value READ getDbusValue WRITE setValue NOTIFY valueChanged)

public:
    IVIVehicleSetting(QObject* parent = nullptr);
    ~IVIVehicleSetting();

    virtual QString key() const = 0;
    virtual QVariant getValue() const = 0;
    virtual void setValue(const QVariant&) = 0;

Q_SIGNALS:
    void valueChanged();

private:
    QVariant getDbusValue() const;
};

QT_END_NAMESPACE

#endif // STEVEHICLESETTING_H
