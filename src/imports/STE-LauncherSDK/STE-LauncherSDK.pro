CXX_MODULE = STE-LauncherSDK
TARGET  = declarative_STE-LauncherSDK
TARGETPATH = solarteameindhoven/launcher
IMPORT_VERSION = 1.0

QT += qml quick quicktemplates2-private waylandclient-private STE-LauncherSDK

OTHER_FILES += qmldir

SOURCES += \
    STELauncherSDK.cpp

HEADERS += 

load(qml_plugin)

for(FILE, qmldir.files) {
    DDIR = $$DESTDIR

    # Replace slashes in paths with backslashes for Windows
    win32:FILE ~= s,/,\\,g
    win32:DDIR ~= s,/,\\,g

    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
}
export(QMAKE_POST_LINK)
