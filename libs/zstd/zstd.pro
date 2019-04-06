TARGET = zstd
VERSION = 1.3.7
TR_EXCLUDE += $$PWD/*
SRC_DIR=$$PWD/$$TARGET/
PRO_DIR=$$PWD/

DEFINES += ZSTD_MULTITHREAD=1
DEFINES += XXH_NAMESPACE=ZSTD_
DEFINES += XXH_PRIVATE_API=1
## if you want this on you have to set to 4
LEGACY=0
DEFINES += ZSTD_LEGACY_SUPPORT=$$LEGACY

include( ../library.pri )
static { DEFINES += ZSTD_BUILD_STATIC=1 }


## set out override as the downloader dont work as the basename of the archs are odd
OTHER_FILES += $$PWD/update-zstd.sh
include( ../downloader.pri )

INCLUDEPATH += \
    $$PWD/zstd/lib \
    $$PWD/zstd/lib/common \

SOURCES += \
    $$PWD/zstd/lib/common/entropy_common.c \
    $$PWD/zstd/lib/common/fse_decompress.c \
    $$PWD/zstd/lib/common/threading.c \
    $$PWD/zstd/lib/common/pool.c \
    $$PWD/zstd/lib/common/zstd_common.c \
    $$PWD/zstd/lib/common/error_private.c \
    $$PWD/zstd/lib/common/xxhash.c \
    $$PWD/zstd/lib/compress/hist.c \
    $$PWD/zstd/lib/compress/fse_compress.c \
    $$PWD/zstd/lib/compress/huf_compress.c \
    $$PWD/zstd/lib/compress/zstd_compress.c \
    $$PWD/zstd/lib/compress/zstdmt_compress.c \
    $$PWD/zstd/lib/compress/zstd_fast.c \
    $$PWD/zstd/lib/compress/zstd_double_fast.c \
    $$PWD/zstd/lib/compress/zstd_lazy.c \
    $$PWD/zstd/lib/compress/zstd_opt.c \
    $$PWD/zstd/lib/compress/zstd_ldm.c \
    $$PWD/zstd/lib/decompress/huf_decompress.c \
    $$PWD/zstd/lib/decompress/zstd_decompress.c \
    $$PWD/zstd/lib/dictBuilder/cover.c \
    $$PWD/zstd/lib/dictBuilder/fastcover.c \
    $$PWD/zstd/lib/dictBuilder/divsufsort.c \
    $$PWD/zstd/lib/dictBuilder/zdict.c \
    $$PWD/zstd/lib/deprecated/zbuff_common.c \
    $$PWD/zstd/lib/deprecated/zbuff_compress.c \
    $$PWD/zstd/lib/deprecated/zbuff_decompress.c


HEADERS += \
    $$PWD/zstd/lib/zstd.h \
    $$PWD/zstd/lib/common/debug.h \
    $$PWD/zstd/lib/common/pool.h \
    $$PWD/zstd/lib/common/threading.h \
    $$PWD/zstd/lib/common/bitstream.h \
    $$PWD/zstd/lib/common/error_private.h \
    $$PWD/zstd/lib/common/zstd_errors.h \
    $$PWD/zstd/lib/common/fse.h \
    $$PWD/zstd/lib/common/huf.h \
    $$PWD/zstd/lib/common/mem.h \
    $$PWD/zstd/lib/common/xxhash.h \
    $$PWD/zstd/lib/common/zstd_internal.h \
    $$PWD/zstd/lib/compress/hist.h \
    $$PWD/zstd/lib/compress/zstd_compress_internal.h \
    $$PWD/zstd/lib/compress/zstd_fast.h \
    $$PWD/zstd/lib/compress/zstd_double_fast.h \
    $$PWD/zstd/lib/compress/zstd_lazy.h \
    $$PWD/zstd/lib/compress/zstd_opt.h \
    $$PWD/zstd/lib/compress/zstd_ldm.h \
    $$PWD/zstd/lib/compress/zstdmt_compress.h \
    $$PWD/zstd/lib/dictBuilder/zdict.h \
    $$PWD/zstd/lib/dictBuilder/cover.h \
    $$PWD/zstd/lib/deprecated/zbuff.h


#IF (ZSTD_LEGACY_SUPPORT)
greaterThan( LEGACY, 0 ){
    INCLUDEPATH += \
        $$PWD/zstd/lib/legacy

    SOURCES += \
        $$PWD/zstd/lib/legacy/zstd_v01.c \
        $$PWD/zstd/lib/legacy/zstd_v02.c \
        $$PWD/zstd/lib/legacy/zstd_v03.c \
        $$PWD/zstd/lib/legacy/zstd_v04.c \
        $$PWD/zstd/lib/legacy/zstd_v05.c \
        $$PWD/zstd/lib/legacy/zstd_v06.c \
        $$PWD/zstd/lib/legacy/zstd_v07.c

    HEADERS += \
        $$PWD/zstd/lib/legacy/zstd_legacy.h \
        $$PWD/zstd/lib/legacy/zstd_v01.h \
        $$PWD/zstd/lib/legacy/zstd_v02.h \
        $$PWD/zstd/lib/legacy/zstd_v03.h \
        $$PWD/zstd/lib/legacy/zstd_v04.h \
        $$PWD/zstd/lib/legacy/zstd_v05.h \
        $$PWD/zstd/lib/legacy/zstd_v06.h \
        $$PWD/zstd/lib/legacy/zstd_v07.h

} else {
    message(no legacy support in zstd)
}

PUBLIC_HEADERS += \
    $$PWD/zstd/lib/zstd.h \
    $$PWD/zstd/lib/common/zstd_errors.h \
    $$PWD/zstd/lib/common/xxhash.h \
    $$PWD/zstd/lib/deprecated/zbuff.h \
    $$PWD/zstd/lib/dictBuilder/zdict.h

include( ../deployment.pri)


