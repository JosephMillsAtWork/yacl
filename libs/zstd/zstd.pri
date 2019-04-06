win32:CONFIG(release, debug|release): LIBS += -L$$SHADOW_DIR/libs/zstd/release/libzstd.a
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SHADOW_DIR/libs/zstd/debug/libzstd.a
else:unix: LIBS += $$SHADOW_DIR/libs/zstd/libzstd.a

INCLUDEPATH += \
    $$PWD/../zstd/zstd/lib \
    $$PWD/../zstd/zstd/lib/common

DEPENDPATH += \
    $$PWD/../zstd/zstd/lib \
    $$PWD/../zstd/zstd/lib/common

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/zstd/release/libzstd.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/zstd/debug/libzstd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/zstd/release/zstd.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/zstd/debug/zstd.lib
else:unix: PRE_TARGETDEPS += $$SHADOW_DIR/libs/zstd/libzstd.a

