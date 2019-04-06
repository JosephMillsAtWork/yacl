TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    $$PWD/zlib/zlib.pro \
    $$PWD/bzip2/bzip2.pro \
    $$PWD/xz/xz.pro \
    $$PWD/lzo/lzo.pro \
    $$PWD/7zip/7zip.pro \
    $$PWD/zip/zip.pro \
    $$PWD/zstd/zstd.pro \
    $$PWD/libarchive/libarchive.pro \
    $$PWD/archive-qt/archive-qt.pro

OTHER_FILES += \
    $$PWD/README.md
