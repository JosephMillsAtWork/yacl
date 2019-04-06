TEMPLATE = aux
TARGET = lzo
CONFIG -= qt
UPSTREAM_URL="http://www.oberhumer.com/opensource/lzo/download/lzo-2.10.tar.gz"
SRC_DIR=$$PWD/$$TARGET
PRO_DIR=$$PWD
BUILD_DIR=$$OUT_PWD
OTHER_FILES += $$PWD/update-lzo.sh


!exists($$PWD/lzo){
    QMAKE_DEBUG: message( lzo2 source folder not found. Will try to download it on qmake )
    QMAKE_DEBUG: message( upstream: $$UPSTREAM )
    system($$PWD/update-lzo.sh)
    exists($$PWD/lzo){
        QMAKE_DEBUG: message( lzo2 source is on disk. to remove run make distclean )
    } else {
         error(could not download lzo)
    }
} else {
    QMAKE_DEBUG: message( lzo2 source is on disk. to remove run make distclean )
}


#configure.target = $$BUILD_DIR/config_test.h
configure.CONFIG += target_predeps

LZO_CONFIG_OPTS = --verbose \
              --prefix=$$INSTALL_PREFIX \
             --disable-shared \
              CC=\"$$QMAKE_CC\" \
              CFLAGS=\"$$QMAKE_CFLAGS\"

CROSS_COMPILING {
    LZO_CONFIG_OPTS += " --host=arm-linux-gnueabihf "
}

win32 {
    win32-mxe{
        LZO_CONFIG_OPTS += " --host=$$MXE_HOST "
    }


    LZO_CONFIG_COMMANDS += \
        " mkdir -p $$BUILD_DIR ; " \
        " cd $$BUILD_DIR; $$SRC_DIR/configure $$LZO_CONFIG_OPTS ; "


} else {
    LZO_CONFIG_COMMANDS = \
        " mkdir -p $$BUILD_DIR ; " \
        " cd $$BUILD_DIR  ; " \
        " $$SRC_DIR/configure $$LZO_CONFIG_OPTS  ; " \
        " cd - ; "
}

configure.commands = $$LZO_CONFIG_COMMANDS
qmake_all.depends = configure
make.commands = "cd $$BUILD_DIR; $(MAKE) ; cd - ;"
make.depends = qmake_all
all.depends = make
QMAKE_DISTCLEAN += Makefile
QMAKE_EXTRA_TARGETS += configure qmake_all make install all

OTHER_FILES += $$PWD/lzo.pri

DISTFILES += \
    update-lzo.sh
