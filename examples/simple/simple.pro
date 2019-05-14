QT -= gui
QT += network core
CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += TEST_COMPRESS=\\\"$$PWD/test\\\"


SOURCES += \
    $$PWD/main.cpp

osx{
    LIBS += \
        -framework CoreServices -liconv
}


LIBS += $$INSTALL_QML/ArchiveQt/libArchiveQt.so
message($$LIBS)

INCLUDEPATH += $$PWD/../../libs/archive-qt
DEPENDPATH += $$PWD/../../libs/archive-qt

linux{
    installPath = $$INSTALL_BINS
} else {
    installPath = $$[QT_INSTALL_EXAMPLES]/$$TARGET
}

target.path = $$installPath
INSTALLS += target


include( $$PWD/../../libs/libarchive/libarchive.pri)
