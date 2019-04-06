UPSTREAM=http://anduin.linuxfromscratch.org/LFS/bzip2-1.0.6.tar.gz

OTHER_FILES += \
    update-bzip2.sh

!exists( $$PWD/bzip2 ){
    QMAKE_DEBUG{
        message( bzip2 source folder not found. Will try to download it on qmake )
        message( upstream: $$UPSTREAM )
    }
    system($$PWD/update-bzip2.sh)
} else {
    QMAKE_DEBUG: message( bzip2 source is on disk. to remove run make distclean )
}

QMAKE_DISTCLEAN += -r $$PWD/bzip2

#OTHER_FILES += $$PWD/bzip2.pri
TEMPLATE = lib
TARGET = bz2
DEFINES += bz2
VERSION = 1.0.6
CONFIG -= qt
CONFIG += static create_prl link_prl

osx{
    *-clang* {
        QMAKE_CFLAGS +=-mmacosx-version-min=$$QMAKE_MACOSX_DEPLOYMENT_TARGET -isysroot $$shell_quote($$QMAKE_MAC_SDK_PATH)
    }
}
win32-mxe{
    ## Bug is line 37,131 bzip2/bzip2.c
    ## we have to replace the #include <sys\sys.h> with #include <sys\sys.h>
    QMAKE_CFLAGS += --sysroot=$$shell_path($$MXE_ROOT)
}
BIGFILES=-D_FILE_OFFSET_BITS=64
QMAKE_CFLAGS += -Wall -Winline -O2 -g $$BIGFILES -fPIC
INPUT_C += \
    $$PWD/bzip2/blocksort.c \
    $$PWD/bzip2/huffman.c \
    $$PWD/bzip2/crctable.c \
    $$PWD/bzip2/randtable.c \
    $$PWD/bzip2/compress.c \
    $$PWD/bzip2/decompress.c \
    $$PWD/bzip2/bzip2.c \
    $$PWD/bzip2/bzlib.c

isEmpty(OBJECTS_DIR):OBJECTS_DIR=$$OUT_PWD
else: message( obj dir was set )
bz_decl.name = bzip2
bz_decl.input = INPUT_C
bz_decl.output = ${QMAKE_FILE_BASE}.o
bz_decl.commands = $$QMAKE_CC $$QMAKE_CFLAGS -c ${QMAKE_FILE_NAME}
bz_decl.variable_out = OBJECTS
QMAKE_EXTRA_COMPILERS += bz_decl

OBJS = blocksort.o
OBJS += \
    huffman.o \
    crctable.o \
    randtable.o \
    compress.o \
    decompress.o \
    bzlib.o

#QMAKE_CLEAN = $$OBJS
#rm -f *~ core *.core
#rm -f libbz2.a
#rm -f -r /Users/josephmills/untitled2/3rdparty/bzip2/bzip2
#rm -f Makefile



AR=$$QMAKE_AR;
isEmpty(AR): AR=ar cq
bz_ar.depends = bz_decl
bz_ar.commands = rm -rf $$PWD/libbz2.a
bz_ar.commands += $$AR libbz2.a $$OBJS
QMAKE_EXTRA_TARGETS += bz_ar

RL=$$QMAKE_RANLIB
isEmpty(RL): RL=ranlib
bz_rl.name = bz2ranlib
bz_rl.depends = bz_ar  bz_decl
cz_rl.commands = $$RL libbz2.a ;
QMAKE_EXTRA_TARGETS += bz_rl

libbz2.name = libbz2.a
libbz2.depends = bz_rl
QMAKE_EXTRA_TARGETS += libbz2

# creates bz2 program
bzip2.depends = libbz2
bzip2.commands = $$QMAKE_CC $$QMAKE_CFLAGS $(LDFLAGS) -o bzip2 $$OBJECTS_DIR/bzip2.o -L. -lbz2
QMAKE_EXTRA_TARGETS += bzip2

# creates bzip2recover
bzip2recover.depends = bzip2
bzip2recover.commands = $$QMAKE_CC $$QMAKE_CFLAGS $(LDFLAGS) -o bzip2recover bzip2recover.o
QMAKE_EXTRA_TARGETS += bzip2recover

check.name = test
check.depends =  bzip2
check.commands = @cat words1
check.commands += ./bzip2 -1  < sample1.ref > sample1.rb2
check.commands += ./bzip2 -2  < sample2.ref > sample2.rb2
#        ./bzip2 -3  < sample3.ref > sample3.rb2
#        ./bzip2 -d  < sample1.bz2 > sample1.tst
#        ./bzip2 -d  < sample2.bz2 > sample2.tst
#        ./bzip2 -ds < sample3.bz2 > sample3.tst
#        cmp sample1.bz2 sample1.rb2
#        cmp sample2.bz2 sample2.rb2
#        cmp sample3.bz2 sample3.rb2
#        cmp sample1.tst sample1.ref
#        cmp sample2.tst sample2.ref
#        cmp sample3.tst sample3.ref
check.commands += @cat words3
QMAKE_EXTRA_TARGETS += check

#all.Config
all.depends = libbz2 bzip2 bzip2recover check

PUBLIC_HEADERS += $$PWD/bzip2/bzlib.h
headers.files = $$PUBLIC_HEADERS
headers.path = $$INSTALL_HEADERS/$$TARGET
target.path = $$INSTALL_LIBS

INSTALLS += \
    target \
    headers

