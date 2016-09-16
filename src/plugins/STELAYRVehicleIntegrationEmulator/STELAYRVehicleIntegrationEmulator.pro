#-------------------------------------------------
#
# Project created by QtCreator 2016-08-21T00:48:10
#
#-------------------------------------------------

QT       += core gui widgets IVI-LauncherSDK
CONFIG   += c++11

TARGET = STELAYRVehicleIntegrationEmulator
TEMPLATE = lib

PLUGIN_TYPE = ste-vehicleinteractionbackend
#PLUGIN_EXTENDS = -
PLUGIN_CLASS_NAME = VehicleIntegrationEmulatorPlugin
load(qt_plugin)

SOURCES += vehicleintegrationemulatorplugin.cpp \
    vehicleinteractionemulatorbackend.cpp \
    vehicledatafield.cpp \
    vehicledata.cpp

HEADERS += vehicleintegrationemulatorplugin.h \
    vehicleinteractionemulatorbackend.h \
    vehicledatafield.h \
    vehicledata.h

DISTFILES += STELAYRVehicleIntegrationEmulator.json
