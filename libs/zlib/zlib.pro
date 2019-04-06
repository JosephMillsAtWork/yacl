TEMPLATE = lib
TARGET = z
DEFINES += zlib
VERSION = 1.23
CONFIG += \
    static \
    create_prl \
    link_prl

INCLUDEPATH += \
    $$PWD/zlib/src

SOURCES+= \
    $$PWD/adler32.c \
    $$PWD/compress.c \
    $$PWD/crc32.c \
    $$PWD/deflate.c \
    $$PWD/gzclose.c \
    $$PWD/gzlib.c \
    $$PWD/gzread.c \
    $$PWD/gzwrite.c \
    $$PWD/infback.c \
    $$PWD/inffast.c \
    $$PWD/inflate.c \
    $$PWD/inftrees.c \
    $$PWD/trees.c \
    $$PWD/uncompr.c \
    $$PWD/zutil.c

TR_EXCLUDE += $$PWD/*
PUBLIC_HEADERS += \
    $$PWD/zlib.h \
    $$PWD/zconf.h \
    $$PWD/zlib_mangle.h


isEmpty(INSTALL_PREFIX): INSTALL_PREFIX=$$OUT_PWD
headers.files = $$HEADERS
win32{
    win32-mxe{
        headers.path = $$INSTALL_PREFIX/include
        target.path = $$INSTALL_PREFIX/lib
    } else {
        headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
        target.path = $$[QT_INSTALL_LIBS]/
    }
}

linux{
    headers.files = $$PUBLIC_HEADERS
    headers.path = $$INSTALL_PREFIX/include
    target.path = $$INSTALL_PREFIX/lib
}

android{ error(not supported yet )}
ios{ error( Not supported yet )}
osx{
    headers.files = $$PUBLIC_HEADERS
    headers.path = $$INSTALL_PREFIX/include
    target.path = $$INSTALL_PREFIX/lib
}
INSTALLS += \
    target \
    headers
