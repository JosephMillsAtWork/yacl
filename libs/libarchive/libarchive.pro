include($$PWD/../zlib/zlib.pri )
include($$PWD/../bzip2/bzip2.pri )
include($$PWD/../xz.pri )
include($$PWD/../lzo/lzo.pri )
include($$PWD/../zip/zip.pri )
include($$PWD/../7zip/7zip.pri )
include($$PWD/../zstd/zstd.pri )

TEMPLATE = lib
TARGET = archive

CONFIG += create_prl link_prl

CONFIG += \
    static \
    hide_symbols \
    exceptions_off \
    rtti_off warn_off \
    installed


DEFINES += LIBARCHIVE _STATIC
DEFINES += HAVE_CONFIG_H=1


osx:LIBS += -framework CoreServices -liconv
win32:LIBS += -lcrypt32
win32:MODULE_DEFINES+=LIBARCHIVE _STATIC
MODULE_INCLUDEPATH+=$$PWD/libarchive

win32-msvc* {
    QMAKE_CFLAGS += /wd4146 /wd4133 /D_CRT_SECURE_NO_WARNINGS
}
*-g++* {
    QMAKE_CFLAGS += -Wno-unused -Wno-sign-compare -Wno-old-style-declaration
}
*-clang* {
    CONFIG *= warn_off
    QMAKE_CFLAGS += -Wall -W -Wno-unused -Wno-sign-compare
}

android:DEFINES += PLATFORM_CONFIG_H=\\\"config-android.h\\\"
win32{
#    win32-g++ {
#        DEFINES += PLATFORM_CONFIG_H=\\\"config-mingw.h\\\"
#    } else {
        DEFINES += PLATFORM_CONFIG_H=\\\"config-windows.h\\\"
#    }
}
else:osx:DEFINES += PLATFORM_CONFIG_H=\\\"config-osx.h\\\"
else:DEFINES += PLATFORM_CONFIG_H=\\\"config-unix.h\\\"
include( $$PWD/libarchive-define-helper.pri )
OTHER_FILES += \
    config-android.h \
    config-windows.h \
    config-osx.h \
    config-unix.h \
    config-mingw.h \
    android_lf.h
#    config-new.h

INCLUDEPATH += \
    $$PWD/ \
    $$PWD/libarchive/libarchive

SOURCES += \
  $$PWD/libarchive/libarchive/archive_acl.c \
  $$PWD/libarchive/libarchive/archive_check_magic.c \
  $$PWD/libarchive/libarchive/archive_cmdline.c \
  $$PWD/libarchive/libarchive/archive_cryptor.c \
  $$PWD/libarchive/libarchive/archive_digest.c \
  $$PWD/libarchive/libarchive/archive_entry.c \
  $$PWD/libarchive/libarchive/archive_entry_copy_stat.c \
  $$PWD/libarchive/libarchive/archive_entry_link_resolver.c \
  $$PWD/libarchive/libarchive/archive_entry_sparse.c \
  $$PWD/libarchive/libarchive/archive_entry_stat.c \
  $$PWD/libarchive/libarchive/archive_entry_strmode.c \
  $$PWD/libarchive/libarchive/archive_entry_xattr.c \
  $$PWD/libarchive/libarchive/archive_getdate.c \
  $$PWD/libarchive/libarchive/archive_hmac.c \
  $$PWD/libarchive/libarchive/archive_match.c \
  $$PWD/libarchive/libarchive/archive_options.c \
  $$PWD/libarchive/libarchive/archive_pack_dev.c \
  $$PWD/libarchive/libarchive/archive_ppmd7.c \
  $$PWD/libarchive/libarchive/archive_rb.c \
  $$PWD/libarchive/libarchive/archive_read.c \
  $$PWD/libarchive/libarchive/archive_read_add_passphrase.c \
  $$PWD/libarchive/libarchive/archive_read_append_filter.c \
  $$PWD/libarchive/libarchive/archive_read_data_into_fd.c \
  $$PWD/libarchive/libarchive/archive_read_disk_entry_from_file.c \
  $$PWD/libarchive/libarchive/archive_read_disk_posix.c \
  $$PWD/libarchive/libarchive/archive_read_disk_set_standard_lookup.c \
  $$PWD/libarchive/libarchive/archive_read_extract.c \
  $$PWD/libarchive/libarchive/archive_read_extract2.c \
  $$PWD/libarchive/libarchive/archive_read_open_fd.c \
  $$PWD/libarchive/libarchive/archive_read_open_file.c \
  $$PWD/libarchive/libarchive/archive_read_open_filename.c \
  $$PWD/libarchive/libarchive/archive_read_open_memory.c \
  $$PWD/libarchive/libarchive/archive_read_set_format.c \
  $$PWD/libarchive/libarchive/archive_read_set_options.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_all.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_bzip2.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_compress.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_gzip.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_grzip.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_lrzip.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_lz4.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_lzop.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_none.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_program.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_rpm.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_uu.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_xz.c \
  $$PWD/libarchive/libarchive/archive_read_support_filter_zstd.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_7zip.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_all.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_ar.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_by_code.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_cab.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_cpio.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_empty.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_iso9660.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_lha.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_mtree.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_rar.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_raw.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_tar.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_warc.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_xar.c \
  $$PWD/libarchive/libarchive/archive_read_support_format_zip.c \
  $$PWD/libarchive/libarchive/archive_string.c \
  $$PWD/libarchive/libarchive/archive_string_sprintf.c \
  $$PWD/libarchive/libarchive/archive_util.c \
  $$PWD/libarchive/libarchive/archive_version_details.c \
  $$PWD/libarchive/libarchive/archive_virtual.c \
  $$PWD/libarchive/libarchive/archive_write.c \
  $$PWD/libarchive/libarchive/archive_write_disk_posix.c \
  $$PWD/libarchive/libarchive/archive_write_disk_set_standard_lookup.c \
  $$PWD/libarchive/libarchive/archive_write_open_fd.c \
  $$PWD/libarchive/libarchive/archive_write_open_file.c \
  $$PWD/libarchive/libarchive/archive_write_open_filename.c \
  $$PWD/libarchive/libarchive/archive_write_open_memory.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_b64encode.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_by_name.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_bzip2.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_compress.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_grzip.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_gzip.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_lrzip.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_lz4.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_lzop.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_none.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_program.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_uuencode.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_xz.c \
  $$PWD/libarchive/libarchive/archive_write_add_filter_zstd.c \
  $$PWD/libarchive/libarchive/archive_write_set_format.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_7zip.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_ar.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_by_name.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_cpio.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_cpio_newc.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_filter_by_ext.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_gnutar.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_iso9660.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_mtree.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_pax.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_raw.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_shar.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_ustar.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_v7tar.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_warc.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_xar.c \
  $$PWD/libarchive/libarchive/archive_write_set_format_zip.c \
  $$PWD/libarchive/libarchive/archive_write_set_options.c \
  $$PWD/libarchive/libarchive/archive_write_set_passphrase.c \
  $$PWD/libarchive/libarchive/filter_fork_posix.c \
  $$PWD/libarchive/libarchive/archive_random.c \
  $$PWD/libarchive/libarchive/archive_pathmatch.c \
  $$PWD/libarchive/libarchive/xxhash.c


HEADERS += \
  $$PWD/libarchive/libarchive/archive_acl_private.h \
  $$PWD/libarchive/libarchive/archive_cmdline_private.h \
  $$PWD/libarchive/libarchive/archive_crc32.h \
  $$PWD/libarchive/libarchive/archive_cryptor_private.h \
  $$PWD/libarchive/libarchive/archive_digest_private.h \
  $$PWD/libarchive/libarchive/archive_endian.h \
  $$PWD/libarchive/libarchive/archive_entry.h \
  $$PWD/libarchive/libarchive/archive_entry_locale.h \
  $$PWD/libarchive/libarchive/archive_entry_private.h \
  $$PWD/libarchive/libarchive/archive_getdate.h \
  $$PWD/libarchive/libarchive/archive_hmac_private.h \
  $$PWD/libarchive/libarchive/archive_openssl_evp_private.h \
  $$PWD/libarchive/libarchive/archive_openssl_hmac_private.h \
  $$PWD/libarchive/libarchive/archive_options_private.h \
  $$PWD/libarchive/libarchive/archive_pathmatch.h \
  $$PWD/libarchive/libarchive/archive_platform.h \
  $$PWD/libarchive/libarchive/archive_pack_dev.h \
  $$PWD/libarchive/libarchive/archive_platform_acl.h \
  $$PWD/libarchive/libarchive/archive_platform_xattr.h \
  $$PWD/libarchive/libarchive/archive_ppmd_private.h \
  $$PWD/libarchive/libarchive/archive_write_disk_private.h \
  $$PWD/libarchive/libarchive/archive_string.h \
  $$PWD/libarchive/libarchive/archive_string_composition.h \
  $$PWD/libarchive/libarchive/archive_read_private.h \
  $$PWD/libarchive/libarchive/archive_read_disk_private.h \
  $$PWD/libarchive/libarchive/archive_rb.h \
  $$PWD/libarchive/libarchive/archive_random_private.h \
  $$PWD/libarchive/libarchive/archive_ppmd7_private.h \
  $$PWD/libarchive/libarchive/archive_private.h \
  $$PWD/libarchive/libarchive/archive_write_private.h \
  $$PWD/libarchive/libarchive/archive_xxhash.h \
  $$PWD/libarchive/libarchive/filter_fork.h \
  $$PWD/config-mingw.h

win32{
    SOURCES += \
        $$PWD/libarchive/libarchive/archive_entry_copy_bhfi.c \
        $$PWD/libarchive/libarchive/archive_read_disk_windows.c \
        $$PWD/libarchive/libarchive/archive_windows.c \
        $$PWD/libarchive/libarchive/archive_write_disk_windows.c \
        $$PWD/libarchive/libarchive/filter_fork_windows.c

    HEADERS += \
        $$PWD/libarchive/libarchive/archive_windows.h
}
osx: SOURCES += $$PWD/libarchive/libarchive/archive_disk_acl_darwin.c
linux: SOURCES += $$PWD/libarchive/libarchive/archive_disk_acl_linux.c

PUBLIC_HEADERS += \
      archive.h
      archive_entry.h

isEmpty(INSTALL_PREFIX): INSTALL_PREFIX=$$OUT_PWD
headers.files = $$PUBLIC_HEADERS
win32{
    headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
    target.path = $$[QT_INSTALL_LIBS]/
}

linux{
    headers.files = $$PUBLIC_HEADERS
    headers.path = $$INSTALL_PREFIX/include
    target.path = $$INSTALL_PREFIX/lib
}

android{ error(not supported yet )}
ios{ error( Not supported yet )}
osx{
    headers.path = $$INSTALL_PREFIX/include
    target.path = $$INSTALL_PREFIX/lib
}
INSTALLS += \
    target \
    headers
