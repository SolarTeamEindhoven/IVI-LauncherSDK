#include "vehicledatafield.h"

#include <QJsonObject>
#include <QMetaEnum>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDebug>

VehicleDataField::VehicleDataField(QObject *parent)
    : QObject(parent)
{
    ;
}

VehicleDataField::VehicleDataField(const QJsonObject& obj, QObject *parent)
    : VehicleDataField(parent)
{
    auto itName = obj.find("name");
    auto itType = obj.find("type");
    auto itValue = obj.find("value");

    if(
            itName == obj.end() || !itName.value().isString() ||
            itType == obj.end() || !itType.value().isString() ||
            itValue == obj.end()
            )
    {
        qWarning() << "Malformed config file for vehicle integration emulation plugin!";
    }
    else
    {
        vehicleData.name = itName.value().toString();
        type = toValueDataTypes(itType.value().toString().toLatin1());
        vehicleData.value = itValue.value().toVariant();

        if(type == INVALID)
        {
            qWarning() << "Type" << itType.value().toString() << "is not a valid type!";
        }
        else
        {
            nameLabel.setText(vehicleData.name);

            switch(type)
            {
            case DOUBLE:
                {
                    QDoubleSpinBox* spinbox = new QDoubleSpinBox();
                    vehicleData.value.convert(QVariant::Double);
                    spinbox->setMinimum(std::numeric_limits<double>::min());
                    spinbox->setMaximum(std::numeric_limits<double>::max());
                    spinbox->setValue(vehicleData.value.value<double>());
                    connect(spinbox, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, &VehicleDataField::updateValueDOUBLE);
                    valueWidget = spinbox;
                }
                break;
            case INT32:
                {
                    QSpinBox* spinbox = new QSpinBox();
                    vehicleData.value.convert(QVariant::Int);
                    spinbox->setMinimum(std::numeric_limits<int32_t>::min());
                    spinbox->setMaximum(std::numeric_limits<int32_t>::max());
                    spinbox->setValue(vehicleData.value.value<int64_t>());
                    connect(spinbox, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, &VehicleDataField::updateValueINT32);
                    valueWidget = spinbox;
                }
                break;
            case UINT32:
                {
                    QSpinBox* spinbox = new QSpinBox();
                    vehicleData.value.convert(QVariant::UInt);
                    spinbox->setMinimum(0);
                    spinbox->setMaximum(std::numeric_limits<int32_t>::max());
                    spinbox->setValue(vehicleData.value.value<uint64_t>());
                    connect(spinbox, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, &VehicleDataField::updateValueUINT32);
                    valueWidget = spinbox;
                }
                break;
            }
        }
    }
}

VehicleData* VehicleDataField::getVehicleData()
{
    return (type == INVALID)? nullptr : &vehicleData;
}

QPair<QWidget*, QWidget*> VehicleDataField::getWidgets()
{
    return {(type == INVALID)? nullptr : &nameLabel, valueWidget};
}

QJsonObject VehicleDataField::toJsonObject()
{
    QJsonObject obj;

    obj.insert("name", vehicleData.name);
    obj.insert("type", fromValueDataTypes(type));
    obj.insert("value", QJsonValue::fromVariant(vehicleData.value));

    return obj;
}

void VehicleDataField::updateValueDOUBLE(double v)
{
    vehicleData.value.setValue(v);
    emit vehicleData.valueChanged(vehicleData.getDbusValue());
}

void VehicleDataField::updateValueINT32(int v)
{
    vehicleData.value.setValue(static_cast<int32_t>(v));
    emit vehicleData.valueChanged(vehicleData.getDbusValue());
}

void VehicleDataField::updateValueUINT32(int v)
{
    vehicleData.value.setValue(static_cast<uint32_t>(v));
    emit vehicleData.valueChanged(vehicleData.getDbusValue());
}

const char* VehicleDataField::fromValueDataTypes(ValueDataTypes v)
{
    static int enumIdx = staticMetaObject.indexOfEnumerator("ValueDataTypes");
    return staticMetaObject.enumerator(enumIdx).valueToKey(v);
}

VehicleDataField::ValueDataTypes VehicleDataField::toValueDataTypes(QByteArray v)
{
    static int enumIdx = staticMetaObject.indexOfEnumerator("ValueDataTypes");
    bool ok;
    ValueDataTypes result = static_cast<ValueDataTypes>(staticMetaObject.enumerator(enumIdx).keyToValue(v.constData(), &ok));

    return ok ? result : INVALID;
}
