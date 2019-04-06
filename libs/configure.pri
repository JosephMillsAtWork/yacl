## cc
isEmpty(CPU){
    CPU=1
}

contains(QT_ARCH, i386): ARCH_SWITCH=i386
else: contains(QT_ARCH, x86_64): ARCH_SWITCH=x86_64
else: contains(QT_ARCH, arm): ARCH_SWITCH=arm

CC=$$QMAKE_CC
CXX=$$QMAKE_CXX
osx{
    *-clang* {
        QMAKE_CFLAGS+=-mmacosx-version-min=10.11 -isysroot $$shell_quote($$QMAKE_MAC_SDK_PATH)
    }
}
win32-msvc* {}
*-g++* {}
*-clang* { }

needs_autotools{
    message( $$BUILD_DIR)

    configure.target = $$BUILD_DIR/config_test.h
    configure.CONFIG += target_predeps

    isEmpty( CONFIG_OPTS ){
        CONFIG_OPTS = "--verbose " \
                      "--prefix=$$INSTALL_PREFIX/" \
                      "CC=$$QMAKE_CC" \
                      "CFLAGS=\"$$QMAKE_CFLAGS\" "
    } else {
        QMAKE_DEBUG: message( config opts override )
    }

    CONFIG_COMMANDS = \
        " mkdir -p $$BUILD_DIR ; " \
        " cd $$BUILD_DIR  ; " \
        " $$SRC_DIR/configure $$CONFIG_OPTS  ; " \
        " cd - ; "

    configure.commands = $$CONFIG_COMMANDS

    qmake_all.depends = configure

    first.files = $$BUILD_DIR/config

    make.commands = "cd $$BUILD_DIR; $(MAKE) -j$$CPU ; cd - ;"
    make.depends = qmake_all
    QMAKE_DISTCLEAN += Makefile
    QMAKE_EXTRA_TARGETS += configure qmake_all make install first
}

needs_cmake{


}
