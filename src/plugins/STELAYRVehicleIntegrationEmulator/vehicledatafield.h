#ifndef VEHICLEDATAFIELD_H
#define VEHICLEDATAFIELD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QVariant>

#include "vehicledata.h"

class QJsonObject;

class VehicleDataField : public QObject
{
    Q_OBJECT
public:
    explicit VehicleDataField(QObject *parent = 0);
    VehicleDataField(const QJsonObject& obj, QObject *parent = 0);

    VehicleData* getVehicleData();
    QPair<QWidget*, QWidget*> getWidgets();

    QJsonObject toJsonObject();

    enum ValueDataTypes
    {
        INVALID,
        DOUBLE,
        INT32,
        UINT32
    };
    Q_ENUM(ValueDataTypes)

private:
    ValueDataTypes type;
    VehicleData vehicleData;

    QLabel nameLabel;
    QWidget* valueWidget;

    void updateValueDOUBLE(double);
    void updateValueINT32(int);
    void updateValueUINT32(int);

    static const char* fromValueDataTypes(ValueDataTypes);
    static ValueDataTypes toValueDataTypes(QByteArray);
};

#endif // VEHICLEDATAFIELD_H
