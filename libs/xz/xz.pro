TARGET = lzma
TEMPLATE = lib
CONFIG += static

DEFINES += HAVE_CONFIG_H=1

CROSS_COMPILE {
    QMAKE_CFLAGS += -std=c99 -fPIC
    QMAKE_CXXFLAGS += -std=c++11
    DEFINES += _POSIX_C_SOURCE=199309L
    LIBS += -lpthread
}

OTHER_FILES += $$PWD/update-xz.sh
XZ_SRCDIR = $$PWD/lzma

exists( $$XZ_SRCDIR/configure ){
    QMAKE_DEBUG: message( xz source is on disk. to remove run make distclean )
}else {
    QMAKE_DEBUG: message( xz not found going to try and download it. )
    system( $$PWD/update-xz.sh )
}

win32-g++{
    LIBS += -lpthread
    QMAKE_CFLAGS += -std=c99 -fPIC
    QMAKE_CXXFLAGS += -std=c++11
    DEFINES += _POSIX_C_SOURCE=199309L
    exists( $$PWD/lzma/src/common/sysdefs.h ){
        QMAKE_DEBUG{
            message( removing the bad sysdef as windows sucks )
            message( this file sucks $$PWD/lzma/src/common/sysdefs.h )
        }
        system( rm -f $$PWD/lzma/src/common/sysdefs.h )
    }
}

QMAKE_DISTCLEAN += -r $$PWD/lzma

INCLUDEPATH += \
    $$PWD/lzma/src/liblzma/rangecoder/ \
    $$PWD/lzma/src/liblzma/simple/ \
    $$PWD/lzma/src/liblzma/delta/ \
    $$PWD/lzma/src/liblzma/common/ \
    $$PWD/lzma/src/liblzma/check/ \
    $$PWD/lzma/src/liblzma/lzma/ \
    $$PWD/lzma/src/liblzma/api/ \
    $$PWD/lzma/src/liblzma/lz/ \
    $$PWD/lzma/src/liblzma/ \
    $$PWD/lzma/src/common/ \
    $$PWD/defines


HEADERS += \
    $$PWD/defines/config.h \
    $$PWD/defines/sysdefs.h

osx: HEADERS += $$PWD/defines/osx-config.h

win32-g++{
    HEADERS += $$PWD/defines/win32-config.h
}

linux{
    imx{
        HEADERS += $$PWD/defines/imx6-config.h
    } else {
        HEADERS += $$PWD/defines/linux-config.h
    }
}



HEADERS += \
    $$PWD/lzma/src/xz/args.h \
    $$PWD/lzma/src/xz/coder.h \
    $$PWD/lzma/src/xz/file_io.h \
    $$PWD/lzma/src/xz/hardware.h \
    $$PWD/lzma/src/xz/list.h \
    $$PWD/lzma/src/xz/main.h \
    $$PWD/lzma/src/xz/message.h \
    $$PWD/lzma/src/xz/mytime.h \
    $$PWD/lzma/src/xz/options.h \
    $$PWD/lzma/src/xz/private.h \
    $$PWD/lzma/src/xz/signals.h \
    $$PWD/lzma/src/xz/suffix.h \
    $$PWD/lzma/src/xz/util.h \
    $$PWD/lzma/src/common/mythread.h \
    $$PWD/lzma/src/common/tuklib_cpucores.h \
    $$PWD/lzma/src/common/tuklib_physmem.h \
    $$PWD/lzma/src/liblzma/api/lzma/base.h \
    $$PWD/lzma/src/liblzma/api/lzma/bcj.h \
    $$PWD/lzma/src/liblzma/api/lzma/block.h \
    $$PWD/lzma/src/liblzma/api/lzma/check.h \
    $$PWD/lzma/src/liblzma/api/lzma/container.h \
    $$PWD/lzma/src/liblzma/api/lzma/delta.h \
    $$PWD/lzma/src/liblzma/api/lzma/filter.h \
    $$PWD/lzma/src/liblzma/api/lzma/hardware.h \
    $$PWD/lzma/src/liblzma/api/lzma/index.h \
    $$PWD/lzma/src/liblzma/api/lzma/index_hash.h \
    $$PWD/lzma/src/liblzma/api/lzma/lzma12.h \
    $$PWD/lzma/src/liblzma/api/lzma/stream_flags.h \
    $$PWD/lzma/src/liblzma/api/lzma/version.h \
    $$PWD/lzma/src/liblzma/api/lzma/vli.h \
    $$PWD/lzma/src/liblzma/api/lzma.h \
    $$PWD/lzma/src/liblzma/check/check.h \
    $$PWD/lzma/src/liblzma/check/crc_macros.h \
    $$PWD/lzma/src/liblzma/common/alone_decoder.h \
    $$PWD/lzma/src/liblzma/common/block_buffer_encoder.h \
    $$PWD/lzma/src/liblzma/common/block_decoder.h \
    $$PWD/lzma/src/liblzma/common/block_encoder.h \
    $$PWD/lzma/src/liblzma/common/common.h \
    $$PWD/lzma/src/liblzma/common/easy_preset.h \
    $$PWD/lzma/src/liblzma/common/filter_common.h \
    $$PWD/lzma/src/liblzma/common/filter_decoder.h \
    $$PWD/lzma/src/liblzma/common/filter_encoder.h \
    $$PWD/lzma/src/liblzma/common/index.h \
    $$PWD/lzma/src/liblzma/common/index_encoder.h \
    $$PWD/lzma/src/liblzma/common/memcmplen.h \
    $$PWD/lzma/src/liblzma/common/outqueue.h \
    $$PWD/lzma/src/liblzma/common/stream_decoder.h \
    $$PWD/lzma/src/liblzma/common/stream_flags_common.h \
    $$PWD/lzma/src/liblzma/delta/delta_common.h \
    $$PWD/lzma/src/liblzma/delta/delta_decoder.h \
    $$PWD/lzma/src/liblzma/delta/delta_encoder.h \
    $$PWD/lzma/src/liblzma/delta/delta_private.h \
    $$PWD/lzma/src/liblzma/lz/lz_decoder.h \
    $$PWD/lzma/src/liblzma/lz/lz_encoder.h \
    $$PWD/lzma/src/liblzma/lz/lz_encoder_hash.h \
    $$PWD/lzma/src/liblzma/lz/lz_encoder_hash_table.h \
    $$PWD/lzma/src/liblzma/lzma/fastpos.h \
    $$PWD/lzma/src/liblzma/lzma/lzma2_decoder.h \
    $$PWD/lzma/src/liblzma/lzma/lzma2_encoder.h \
    $$PWD/lzma/src/liblzma/lzma/lzma_common.h \
    $$PWD/lzma/src/liblzma/lzma/lzma_decoder.h \
    $$PWD/lzma/src/liblzma/lzma/lzma_encoder.h \
    $$PWD/lzma/src/liblzma/lzma/lzma_encoder_private.h \
    $$PWD/lzma/src/liblzma/rangecoder/price.h \
    $$PWD/lzma/src/liblzma/rangecoder/range_common.h \
    $$PWD/lzma/src/liblzma/rangecoder/range_decoder.h \
    $$PWD/lzma/src/liblzma/rangecoder/range_encoder.h \
    $$PWD/lzma/src/liblzma/simple/simple_coder.h \
    $$PWD/lzma/src/liblzma/simple/simple_decoder.h \
    $$PWD/lzma/src/liblzma/simple/simple_encoder.h \
    $$PWD/lzma/src/liblzma/simple/simple_private.h


SOURCES += \
    $$PWD/lzma/src/common/tuklib_cpucores.c \
    $$PWD/lzma/src/common/tuklib_physmem.c \
    $$PWD/lzma/src/liblzma/check/check.c \
    $$PWD/lzma/src/liblzma/check/crc32_fast.c \
    $$PWD/lzma/src/liblzma/check/crc32_table.c \
    $$PWD/lzma/src/liblzma/check/crc32_tablegen.c \
    $$PWD/lzma/src/liblzma/check/sha256.c \
    $$PWD/lzma/src/liblzma/common/alone_decoder.c \
    $$PWD/lzma/src/liblzma/common/alone_encoder.c \
    $$PWD/lzma/src/liblzma/common/auto_decoder.c \
    $$PWD/lzma/src/liblzma/common/block_buffer_decoder.c \
    $$PWD/lzma/src/liblzma/common/block_buffer_encoder.c \
    $$PWD/lzma/src/liblzma/common/block_decoder.c \
    $$PWD/lzma/src/liblzma/common/block_encoder.c \
    $$PWD/lzma/src/liblzma/common/block_header_decoder.c \
    $$PWD/lzma/src/liblzma/common/block_header_encoder.c \
    $$PWD/lzma/src/liblzma/common/block_util.c \
    $$PWD/lzma/src/liblzma/common/common.c \
    $$PWD/lzma/src/liblzma/common/easy_buffer_encoder.c \
    $$PWD/lzma/src/liblzma/common/easy_decoder_memusage.c \
    $$PWD/lzma/src/liblzma/common/easy_encoder.c \
    $$PWD/lzma/src/liblzma/common/easy_encoder_memusage.c \
    $$PWD/lzma/src/liblzma/common/easy_preset.c \
    $$PWD/lzma/src/liblzma/common/filter_buffer_decoder.c \
    $$PWD/lzma/src/liblzma/common/filter_buffer_encoder.c \
    $$PWD/lzma/src/liblzma/common/filter_common.c \
    $$PWD/lzma/src/liblzma/common/filter_decoder.c \
    $$PWD/lzma/src/liblzma/common/filter_encoder.c \
    $$PWD/lzma/src/liblzma/common/filter_flags_decoder.c \
    $$PWD/lzma/src/liblzma/common/filter_flags_encoder.c \
    $$PWD/lzma/src/liblzma/common/hardware_cputhreads.c \
    $$PWD/lzma/src/liblzma/common/hardware_physmem.c \
    $$PWD/lzma/src/liblzma/common/index.c \
    $$PWD/lzma/src/liblzma/common/index_decoder.c \
    $$PWD/lzma/src/liblzma/common/index_encoder.c \
    $$PWD/lzma/src/liblzma/common/index_hash.c \
    $$PWD/lzma/src/liblzma/common/outqueue.c \
    $$PWD/lzma/src/liblzma/common/stream_buffer_decoder.c \
    $$PWD/lzma/src/liblzma/common/stream_buffer_encoder.c \
    $$PWD/lzma/src/liblzma/common/stream_decoder.c \
    $$PWD/lzma/src/liblzma/common/stream_encoder.c \
    $$PWD/lzma/src/liblzma/common/stream_encoder_mt.c \
    $$PWD/lzma/src/liblzma/common/stream_flags_common.c \
    $$PWD/lzma/src/liblzma/common/stream_flags_decoder.c \
    $$PWD/lzma/src/liblzma/common/stream_flags_encoder.c \
    $$PWD/lzma/src/liblzma/common/vli_decoder.c \
    $$PWD/lzma/src/liblzma/common/vli_encoder.c \
    $$PWD/lzma/src/liblzma/common/vli_size.c \
    $$PWD/lzma/src/liblzma/delta/delta_common.c \
    $$PWD/lzma/src/liblzma/delta/delta_decoder.c \
    $$PWD/lzma/src/liblzma/delta/delta_encoder.c \
    $$PWD/lzma/src/liblzma/lz/lz_decoder.c \
    $$PWD/lzma/src/liblzma/lz/lz_encoder.c \
    $$PWD/lzma/src/liblzma/lz/lz_encoder_mf.c \
    $$PWD/lzma/src/liblzma/lzma/fastpos_table.c \
    $$PWD/lzma/src/liblzma/lzma/fastpos_tablegen.c \
    $$PWD/lzma/src/liblzma/lzma/lzma2_decoder.c \
    $$PWD/lzma/src/liblzma/lzma/lzma2_encoder.c \
    $$PWD/lzma/src/liblzma/lzma/lzma_decoder.c \
    $$PWD/lzma/src/liblzma/lzma/lzma_encoder.c \
    $$PWD/lzma/src/liblzma/lzma/lzma_encoder_optimum_fast.c \
    $$PWD/lzma/src/liblzma/lzma/lzma_encoder_optimum_normal.c \
    $$PWD/lzma/src/liblzma/lzma/lzma_encoder_presets.c \
    $$PWD/lzma/src/liblzma/rangecoder/price_table.c \
    $$PWD/lzma/src/liblzma/rangecoder/price_tablegen.c \
    $$PWD/lzma/src/liblzma/simple/arm.c \
    $$PWD/lzma/src/liblzma/simple/armthumb.c \
    $$PWD/lzma/src/liblzma/simple/ia64.c \
    $$PWD/lzma/src/liblzma/simple/powerpc.c \
    $$PWD/lzma/src/liblzma/simple/simple_coder.c \
    $$PWD/lzma/src/liblzma/simple/simple_decoder.c \
    $$PWD/lzma/src/liblzma/simple/simple_encoder.c \
    $$PWD/lzma/src/liblzma/simple/sparc.c \
    $$PWD/lzma/src/liblzma/simple/x86.c


headers.files = $$PUBLIC_HEADERS
headers.path = $$INSTALL_HEADERS/$$TARGET
target.path = $$INSTALL_LIBS


INSTALLS += \
    target \
    headers
