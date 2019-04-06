QT -= gui
QT += network core
CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += main.cpp

#include( $$TOP_PWD/3rdparty/zlib/zlib.pri )
#include( $$TOP_PWD/3rdparty/bzip2/bzip2.pri )
#include( $$TOP_PWD/3rdparty/xz/xz.pri )
##include( $$TOP_PWD/3rdparty/lz4/lz4.pri )
#include( $$TOP_PWD/3rdparty/zstd/zstd.pri )
#include( $$TOP_PWD/3rdparty/libarchive/libarchive.pri )
osx: LIBS += -framework CoreServices -liconv


linux{
    installPath = $$INSTALL_BINS
} else {
    installPath = $$[QT_INSTALL_EXAMPLES]/$$TARGET
}

target.path = $$installPath
INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/archive-qt/release/ -larchive-qt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/archive-qt/debug/ -larchive-qt
else:unix: LIBS += -L$$OUT_PWD/../../libs/archive-qt/ -larchive-qt

INCLUDEPATH += $$PWD/../../libs/archive-qt
DEPENDPATH += $$PWD/../../libs/archive-qt

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../libs/archive-qt/release/libarchive-qt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../libs/archive-qt/debug/libarchive-qt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../libs/archive-qt/release/archive-qt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../libs/archive-qt/debug/archive-qt.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../libs/archive-qt/libarchive-qt.a
