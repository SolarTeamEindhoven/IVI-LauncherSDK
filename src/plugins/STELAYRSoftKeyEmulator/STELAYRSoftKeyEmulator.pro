#-------------------------------------------------
#
# Project created by QtCreator 2016-07-08T20:41:33
#
#-------------------------------------------------

QT       += core gui gui-private STE-LauncherSDK quickwidgets
CONFIG   += c++11

TARGET = STELAYRSoftKeyEmulator
TEMPLATE = lib
#CONFIG += plugin

#DESTDIR = $$[QT_INSTALL_PLUGINS]/generic
PLUGIN_TYPE = ste-softkey
#PLUGIN_EXTENDS = -
PLUGIN_CLASS_NAME = SoftKeyProviderPlugin
load(qt_plugin)

SOURCES += softkeyproviderplugin.cpp \
    layrsoftkeyemulatorprovider.cpp \
    layronscreenbuttonsoftkey.cpp \
    layrcontroller.cpp

HEADERS += softkeyproviderplugin.h \
    layrsoftkeyemulatorprovider.h \
    layronscreenbuttonsoftkey.h \
    layrcontroller.h

OTHER_FILES += STELAYRSoftKeyEmulator.json
