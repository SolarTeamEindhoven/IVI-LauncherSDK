TARGET = IVI-Launcher
QT = core core-private dbus qml quick quick-private waylandcompositor xml

CONFIG += c++14

MODULE = IVI-Launcher
MODULE_PLUGIN_TYPES = ivi-vehicleinteractionbackend
load(qt_module)

DEFINES += QT_BUILD_IVI_LAUNCHER_LIB

HEADERS += \
    ivilauncherglobal.h \
    iviappcontainer.h \
    iviappcontainer_p.h \
    ivisurfacemanager.h \
    ivisurfacemanager_p.h \
    iviapplication.h \
    iviapplication_p.h \
    ivimanifest.h \
    ivimanifest_p.h \
    iviapplicationmanager.h \
    iviapplicationmanager_p.h \
    ivivehicledata.h \
    ivivehicledata_p.h \
    ivivehicleinteractionbackend.h \
    ivivehicleinteractionbackendplugin.h \
    ivivehiclesetting.h \
    ivivehiclesetting_p.h \
    ivisingletonmanager_p.h \
    qwaylandquickcompositorquickextensioncontainer_p.h \
    ivilauncherqml_p.h \
    ividbusmanager_p.h \
    ivivehicleinteractionbackendfactory_p.h \
    iviappprocessmanager_p.h \
    iviabstractappcontainer.h

SOURCES += \
    iviappcontainer.cpp \
    ivisurfacemanager.cpp \
    iviapplication.cpp \
    ivimanifest.cpp \
    iviapplicationmanager.cpp \
    ivivehicledata.cpp \
    ivivehicleinteractionbackend.cpp \
    ivivehicleinteractionbackendfactory.cpp \
    ivivehicleinteractionbackendplugin.cpp \
    ivivehiclesetting.cpp \
    ivisingletonmanager.cpp \
    qwaylandquickcompositorquickextensioncontainer.cpp \
    ivilauncherqml.cpp \
    ividbusmanager.cpp \
    iviappprocessmanager.cpp \
    iviabstractappcontainer.cpp

DBUS_ADAPTORS += ../protocol/DBUSManager.xml ../protocol/DBUSVehicleData.xml ../protocol/DBUSVehicleSetting.xml
