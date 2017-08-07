#ifndef STEVEHICLESETTING_H
#define STEVEHICLESETTING_H

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class VehiclesettingAdaptor;

class STEVehicleSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ getDbusValue WRITE setValue NOTIFY valueChanged)

public:
    STEVehicleSetting(QObject* parent = nullptr);
    ~STEVehicleSetting();

    virtual QString key() const = 0;
    virtual QVariant getValue() const = 0;
    virtual void setValue(const QVariant&) = 0;

Q_SIGNALS:
    void valueChanged();

private:
    VehiclesettingAdaptor* adaptor;

    QVariant getDbusValue() const;
};

QT_END_NAMESPACE

#endif // STEVEHICLESETTING_H
