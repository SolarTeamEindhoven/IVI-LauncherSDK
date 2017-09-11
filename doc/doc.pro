TEMPLATE = aux

build_online_docs: {
    QMAKE_DOCS_TARGETDIR = IVI-Launcher
    QMAKE_DOCS = $$PWD/config/IVI-Launcher-online.qdocconf
} else {
    QMAKE_DOCS = $$PWD/config/IVI-Launcher.qdocconf
}

QMAKE_DOCS_OUTPUTDIR = $$OUT_PWD/IVI-Launcher
