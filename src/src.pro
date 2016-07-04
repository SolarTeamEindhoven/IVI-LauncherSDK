TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    STE-LauncherSDK \
    plugins

qtHaveModule(quick) {
    src_imports.subdir = imports
    src_imports.depends = STE-LauncherSDK
    SUBDIRS += src_imports
}
