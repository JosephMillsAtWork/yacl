win32:CONFIG(release, debug|release): LIBS += $$SHADOW_DIR/libs/zip/release/libzip.a
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SHADOW_DIR/libs/zip/debug/libzip.a
else:unix: LIBS += $$SHADOW_DIR/libs/zip/libzip.a
INCLUDEPATH += $$PWD/../zip/zip/lib
DEPENDPATH += $$PWD/../zip/zip/lib
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/zip/release/libzip.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/zip/debug/libzip.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/zip/release/zip.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/zip/debug/zip.lib
else:unix: PRE_TARGETDEPS += $$SHADOW_DIR/libs/zip/libzip.a

