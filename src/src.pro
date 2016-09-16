TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    IVI-LauncherSDK \
    plugins

qtHaveModule(quick) {
    src_imports.subdir = imports
    src_imports.depends = IVI-LauncherSDK
    SUBDIRS += src_imports
}
