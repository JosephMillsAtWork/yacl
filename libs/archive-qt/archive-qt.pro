TEMPLATE = lib
TARGET = ArchiveQt
QT += \
    core \
    network \
    qml \
    quick

CONFIG += \
    c++11 \
    plugin

uri = ArchiveQt

ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ArchiveQt
    CONFIG += static
}

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/archiveError.h \
    $$PWD/compressor.h \
    $$PWD/decompressor.h \
    $$PWD/archivefilesmodel.h \
    $$PWD/archivefile.h \
    $$PWD/decompressionoptions.h \

SOURCES += \
    $$PWD/compressor.cpp \
    $$PWD/decompressor.cpp \
    $$PWD/archivefilesmodel.cpp \
    $$PWD/archivefile.cpp \
    $$PWD/decompressionoptions.cpp


DISTFILES += $$PWD/qmldir

headers.files = $$HEADERS
headers.CONFIG += no_check_exist
headers.path = $$INSTALL_HEADERS/$$TARGET

qmlfiles.files = $$DISTFILES
qmlfiles.path = $$INSTALL_QML/$$TARGET

target.path = $$INSTALL_QML/$$TARGET


INSTALLS += \
    target \
    headers \
    qmlfiles

include( $$PWD/../libarchive/libarchive.pri)
