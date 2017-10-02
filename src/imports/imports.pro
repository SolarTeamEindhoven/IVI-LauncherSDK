CXX_MODULE = qml
TARGET = IVILauncherPlugin
TARGETPATH = ivi/launcher/
IMPORT_VERSION = 1.0

QT += qml IVI-Launcher IVI-Launcher-private
CONFIG += C++11

SOURCES += pluginmain.cpp \
    iviapplicationmanagerqml.cpp \
    iviappcontainerqml.cpp

OTHER_FILES += qmldir

load(qml_plugin)

HEADERS += \
    iviapplicationmanagerqml.h \
    iviappcontainerqml.h
