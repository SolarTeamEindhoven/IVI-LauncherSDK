#include "vehicleinteractionemulatorbackend.h"

#include "vehicledatafield.h"

#include <QMenuBar>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDebug>

VehicleInteractionEmulatorBackend::VehicleInteractionEmulatorBackend(QObject *parent)
    : STEVehicleInteractionBackend(parent)
    , file("vehicledata.json")
{
    QMenuBar* menuBar = window.menuBar();
    QMenu* editMenu = menuBar->addMenu("&Edit");
    editMenu->addAction("Add Data Field", this, &VehicleInteractionEmulatorBackend::addDataField);

    centerWidget.setLayout(&layout);
    window.setCentralWidget(&centerWidget);

    if(file.open(QFile::ReadWrite))
    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
        if(error.error == QJsonParseError::NoError)
        {
            QJsonArray array = document.array();
            foreach(QJsonValue value, array)
            {
                VehicleDataField* v = new VehicleDataField(value.toObject());
                vehicleDataFields.append(v);
                QPair<QWidget*, QWidget*> widgets = v->getWidgets();
                if(widgets.first!= nullptr && widgets.second != nullptr)
                {
                    int row = layout.rowCount();
                    layout.addWidget(widgets.first, row, 0);
                    layout.addWidget(widgets.second, row, 1);
                }
            }
        }
        else
        {
            qWarning() << "Could not interpret" << file.fileName() << "as JSON document:" << error.errorString();
        }
    }

    window.setWindowTitle("Vehicle Integration Emulator");
    window.show();
}

VehicleInteractionEmulatorBackend::~VehicleInteractionEmulatorBackend()
{
    if(file.isOpen())
    {
        QJsonArray array;

        foreach(VehicleDataField* vehicleDataField, vehicleDataFields)
        {
            array.append(vehicleDataField->toJsonObject());
        }

        file.seek(0);
        file.write(QJsonDocument(array).toJson());
    }
}

QList<STEVehicleData*> VehicleInteractionEmulatorBackend::createVehicleDataObjects()
{
    QList<STEVehicleData*> result;

    foreach(VehicleDataField* vehicleDataField, vehicleDataFields)
    {
        VehicleData* vehicleData = vehicleDataField->getVehicleData();
        if(vehicleData != nullptr)
            result.append(vehicleData);
    }

    return result;
}

void VehicleInteractionEmulatorBackend::addDataField()
{
    QJsonObject obj;
    {
        QDialog dialog(&window);
        QFormLayout form(&dialog);
        QLineEdit lineEdit;
        QComboBox comboBox;
        comboBox.insertItems(0, {"Floating point number", "Signed number", "Unsigned number"});
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

        form.addRow(new QLabel("Add data field"));
        form.addRow(new QLabel("Name:"), &lineEdit);
        form.addRow(new QLabel("Type:"), &comboBox);
        form.addRow(&buttonBox);

        QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if(dialog.exec() == QDialog::Accepted)
        {
            obj.insert("name", lineEdit.text());
            switch(comboBox.currentIndex())
            {
            case 0:
                obj.insert("type", "DOUBLE");
                break;
            case 1:
                obj.insert("type", "INT32");
                break;
            case 2:
                obj.insert("type", "UIN32");
                break;
            }
            obj.insert("value", "");
            VehicleDataField* v = new VehicleDataField(obj);
            vehicleDataFields.append(v);
            QPair<QWidget*, QWidget*> widgets = v->getWidgets();
            if(widgets.first!= nullptr && widgets.second != nullptr)
            {
                int row = layout.rowCount();
                layout.addWidget(widgets.first, row, 0);
                layout.addWidget(widgets.second, row, 1);
            }
        }
    }
}
