TARGET     = STE-LauncherSDK
QT         = core core-private qml quick waylandcompositor xml dbus
CONFIG    += c++11 wayland-scanner

MODULE_PLUGIN_TYPES = ste-softkey

load(qt_module)

DEFINES += QT_BUILD_STE_LAUNCHERSDK_LIB

QMAKE_DOCS = $$PWD/doc/STE-LauncherSDK.qdocconf

DBUS_ADAPTORS += ../protocol/DBUSManager.xml ../protocol/DBUSVehicleData.xml

HEADERS += qtstelaunchersdkglobal.h \
    stecompositor.h \
    steappcontainer.h \
    steappinstance.h \
    steapp.h \
    steappmanager.h \
    steappprocessmanager.h \
    stemanifestparser.h \
    steappinstancemanaged.h \
    stesoftkey.h \
    wayland/steshell_wl_p.h \
    wayland/steshellsurface_wl_p.h \
    wayland/stesoftkey_wl_p.h \
    stesoftkeyplugin.h \
    stesoftkeyfactory.h \
    stesoftkeyprovider.h \
    stesoftkeymanager.h \
    stedbusmanager_p.h \
    stevehicledata.h \
    stevehicleinteractionbackendfactory.h \
    stevehicleinteractionbackend.h \
    stevehicleinteractionbackendplugin.h

SOURCES += \ 
    wayland/steshell_wl.cpp \
    wayland/steshellsurface_wl.cpp \
    wayland/stesoftkey_wl.cpp \
    stecompositor.cpp \
    steappcontainer.cpp \
    steappinstance.cpp \
    steapp.cpp \
    steappmanager.cpp \
    steappprocessmanager.cpp \
    stemanifestparser.cpp \
    steappinstancemanaged.cpp \
    stesoftkey.cpp \
    stesoftkeyplugin.cpp \
    stesoftkeyfactory.cpp \
    stesoftkeyprovider.cpp \
    stesoftkeymanager.cpp \
    stedbusmanager.cpp \
    stevehicledata.cpp \
    stevehicleinteractionbackendfactory.cpp \
    stevehicleinteractionbackend.cpp \
    stevehicleinteractionbackendplugin.cpp

contains(QT_CONFIG, no-pkg-config) {
    LIBS += -lwayland-server
} else {
    CONFIG += link_pkgconfig
    PKGCONFIG += wayland-server
}

WAYLANDSERVERSOURCES += ../protocol/STE.xml
