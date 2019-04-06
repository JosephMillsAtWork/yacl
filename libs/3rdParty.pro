TEMPLATE = subdirs
CONFIG += ordered
OTHER_FILES += .gitignore
SUBDIRS += \
    $$PWD/zlib/zlib.pro \
    $$PWD/bzip2/bzip2.pro \
    $$PWD/xz/xz.pro \
    $$PWD/lzo/lzo.pro \
    $$PWD/7zip/7zip.pro \
    $$PWD/zip/zip.pro \
    $$PWD/zstd/zstd.pro \
    $$PWD/libarchive/libarchive.pro \
    $$PWD/botan/botan.pro \
    $$PWD/protobuf/protobuf.pro
