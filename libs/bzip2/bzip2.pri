win32:CONFIG(release, debug|release): LIBS += -L$$SHADOW_DIR/libs/bzip2/release/ -lbz2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SHADOW_DIR/libs/bzip2/debug/ -lbz2
else:unix: LIBS += $$SHADOW_DIR/libs/bzip2/libbz2.a
INCLUDEPATH += $$PWD/../bzip2/bzip2
DEPENDPATH += $$PWD/../bzip2/bzip2
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/bzip2/release/libbz2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/bzip2/debug/libbz2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/bzip2/release/bz2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/bzip2/debug/bz2.lib
else:unix: PRE_TARGETDEPS += $$SHADOW_DIR/libs/bzip2/libbz2.a