TEMPLATE = lib
TARGET = archive-qt
QT += core network

CONFIG += \
    c++11 \
    static \
    create_prl \
    link_prl

HEADERS += \
    $$PWD/downloadmanager.h \
    $$PWD/global.h \
    $$PWD/error.h \
    $$PWD/exception.h \
    $$PWD/extractor_p.h \
    $$PWD/extractor.h \
    $$PWD/compressor.h

#    packagecreator_p.h \
SOURCES += \
    $$PWD/downloadmanager.cpp \
    $$PWD/exception.cpp \
    $$PWD/extractor.cpp \
    $$PWD/compressor.cpp


osx: LIBS += -framework CoreServices -liconv
headers.files = $$HEADERS
headers.CONFIG += no_check_exist
headers.path = $$INSTALL_HEADERS/$$TARGET
target.path = $$INSTALL_LIBS

INSTALLS += \
    target \
    headers

include( $$PWD/../libarchive/libarchive.pri)
