UPSTREAM=https://libzip.org/download/libzip-1.5.1.tar.gz
OTHER_FILES += $$PWD/update-libzip.sh
exists( $$PWD/libzip ) {
#    message( zip source is on disk. to remove run make distclean )
} else {
#    message( libzip not found going to try and download it. )
    win32{
        QMAKE_DEBUG: message($$PWD/update-zip.sh)
        system( $$PWD/update-zip.sh )
#        else : system( c:\msys64\msys2_shell.cmd -mingw64 -c $$PWD/update-libzip.sh )
    } else {
        system( $$PWD/update-zip.sh )
    }
    exists( $$PWD/libzip ){
       QMAKE_DEBUG: message( libzip download success )
    } else {
        error( still no source code  )
    }
}
QMAKE_DISTCLEAN += -r $$PWD/libzip

TEMPLATE = lib
TARGET = zip
VERSION = 1.5.1
CONFIG += static

include( defines.pri )
include( ../bzip2/bzip2.pri )
include( ../zlib/zlib.pri )

HEADERS += \
    $$PWD/libzip/lib/zip.h \
    $$PWD/zipconf.h \
    $$PWD/config.h

INCLUDEPATH += $$PWD
#INCLUDEPATH += $$PWD/libzip/lib

SOURCES += \
    $$PWD/libzip/lib/zip_add_dir.c \
    $$PWD/libzip/lib/zip_add_entry.c \
    $$PWD/libzip/lib/zip_algorithm_deflate.c \
    $$PWD/libzip/lib/zip_buffer.c \
    $$PWD/libzip/lib/zip_add.c \
    $$PWD/libzip/lib/zip_close.c \
    $$PWD/libzip/lib/zip_delete.c \
    $$PWD/libzip/lib/zip_dir_add.c \
    $$PWD/libzip/lib/zip_dirent.c \
    $$PWD/libzip/lib/zip_discard.c \
    $$PWD/libzip/lib/zip_entry.c \
    $$PWD/libzip/lib/zip_err_str.c \
    $$PWD/libzip/lib/zip_error.c \
    $$PWD/libzip/lib/zip_error_clear.c \
    $$PWD/libzip/lib/zip_error_get.c \
    $$PWD/libzip/lib/zip_error_get_sys_type.c \
    $$PWD/libzip/lib/zip_error_strerror.c \
    $$PWD/libzip/lib/zip_error_to_str.c \
    $$PWD/libzip/lib/zip_extra_field.c \
    $$PWD/libzip/lib/zip_extra_field_api.c \
    $$PWD/libzip/lib/zip_fclose.c \
    $$PWD/libzip/lib/zip_fdopen.c \
    $$PWD/libzip/lib/zip_file_add.c \
    $$PWD/libzip/lib/zip_file_error_clear.c \
    $$PWD/libzip/lib/zip_file_error_get.c \
    $$PWD/libzip/lib/zip_file_get_comment.c \
    $$PWD/libzip/lib/zip_file_get_external_attributes.c \
    $$PWD/libzip/lib/zip_file_get_offset.c \
    $$PWD/libzip/lib/zip_file_rename.c \
    $$PWD/libzip/lib/zip_file_replace.c \
    $$PWD/libzip/lib/zip_file_set_comment.c \
    $$PWD/libzip/lib/zip_file_set_encryption.c \
    $$PWD/libzip/lib/zip_file_set_external_attributes.c \
    $$PWD/libzip/lib/zip_file_set_mtime.c \
    $$PWD/libzip/lib/zip_file_strerror.c \
    $$PWD/libzip/lib/zip_filerange_crc.c \
    $$PWD/libzip/lib/zip_fopen.c \
    $$PWD/libzip/lib/zip_fopen_encrypted.c \
    $$PWD/libzip/lib/zip_fopen_index.c \
    $$PWD/libzip/lib/zip_fopen_index_encrypted.c \
    $$PWD/libzip/lib/zip_fread.c \
    $$PWD/libzip/lib/zip_fseek.c \
    $$PWD/libzip/lib/zip_ftell.c \
    $$PWD/libzip/lib/zip_get_archive_comment.c \
    $$PWD/libzip/lib/zip_get_archive_flag.c \
    $$PWD/libzip/lib/zip_get_encryption_implementation.c \
    $$PWD/libzip/lib/zip_get_file_comment.c \
    $$PWD/libzip/lib/zip_get_name.c \
    $$PWD/libzip/lib/zip_get_num_entries.c \
    $$PWD/libzip/lib/zip_get_num_files.c \
    $$PWD/libzip/lib/zip_hash.c \
    $$PWD/libzip/lib/zip_io_util.c \
    $$PWD/libzip/lib/zip_libzip_version.c \
    $$PWD/libzip/lib/zip_memdup.c \
    $$PWD/libzip/lib/zip_name_locate.c \
    $$PWD/libzip/lib/zip_new.c \
    $$PWD/libzip/lib/zip_open.c \
    $$PWD/libzip/lib/zip_progress.c \
    $$PWD/libzip/lib/zip_rename.c \
    $$PWD/libzip/lib/zip_replace.c \
    $$PWD/libzip/lib/zip_set_archive_comment.c \
    $$PWD/libzip/lib/zip_set_archive_flag.c \
    $$PWD/libzip/lib/zip_set_default_password.c \
    $$PWD/libzip/lib/zip_set_file_comment.c \
    $$PWD/libzip/lib/zip_set_file_compression.c \
    $$PWD/libzip/lib/zip_set_name.c \
    $$PWD/libzip/lib/zip_source_begin_write.c \
    $$PWD/libzip/lib/zip_source_begin_write_cloning.c \
    $$PWD/libzip/lib/zip_source_buffer.c \
    $$PWD/libzip/lib/zip_source_call.c \
    $$PWD/libzip/lib/zip_source_close.c \
    $$PWD/libzip/lib/zip_source_commit_write.c \
    $$PWD/libzip/lib/zip_source_compress.c \
    $$PWD/libzip/lib/zip_source_crc.c \
    $$PWD/libzip/lib/zip_source_error.c \
    $$PWD/libzip/lib/zip_source_filep.c \
    $$PWD/libzip/lib/zip_source_free.c \
    $$PWD/libzip/lib/zip_source_function.c \
    $$PWD/libzip/lib/zip_source_get_compression_flags.c \
    $$PWD/libzip/lib/zip_source_is_deleted.c \
    $$PWD/libzip/lib/zip_source_layered.c \
    $$PWD/libzip/lib/zip_source_open.c \
    $$PWD/libzip/lib/zip_source_pkware.c \
    $$PWD/libzip/lib/zip_source_read.c \
    $$PWD/libzip/lib/zip_source_remove.c \
    $$PWD/libzip/lib/zip_source_rollback_write.c \
    $$PWD/libzip/lib/zip_source_seek.c \
    $$PWD/libzip/lib/zip_source_seek_write.c \
    $$PWD/libzip/lib/zip_source_stat.c \
    $$PWD/libzip/lib/zip_source_supports.c \
    $$PWD/libzip/lib/zip_source_tell.c \
    $$PWD/libzip/lib/zip_source_tell_write.c \
    $$PWD/libzip/lib/zip_source_window.c \
    $$PWD/libzip/lib/zip_source_write.c \
    $$PWD/libzip/lib/zip_source_zip.c \
    $$PWD/libzip/lib/zip_source_zip_new.c \
    $$PWD/libzip/lib/zip_stat.c \
    $$PWD/libzip/lib/zip_stat_index.c \
    $$PWD/libzip/lib/zip_stat_init.c \
    $$PWD/libzip/lib/zip_strerror.c \
    $$PWD/libzip/lib/zip_string.c \
    $$PWD/libzip/lib/zip_unchange.c \
    $$PWD/libzip/lib/zip_unchange_all.c \
    $$PWD/libzip/lib/zip_unchange_archive.c \
    $$PWD/libzip/lib/zip_unchange_data.c \
    $$PWD/libzip/lib/zip_utf-8.c

msvc{
    SOURCES += \
        $$PWD/libzip/lib/zip_source_win32handle.c \
        $$PWD/libzip/lib/zip_source_win32utf8.c \
        $$PWD/libzip/lib/zip_source_win32w.c \
        $$PWD/libzip/lib/zip_source_file.c
}

SOURCES += \
    $$PWD/libzip/lib/zip_algorithm_bzip2.c


PUBLIC_HEADERS += $$PWD/libzip/lib/zip.h

headers.files = $$PUBLIC_HEADERS
headers.path = $$INSTALL_HEADERS/$$TARGET
target.path = $$INSTALL_LIBS

INSTALLS += \
    target \
    headers

