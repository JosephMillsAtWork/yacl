win32:CONFIG(release, debug|release): LIBS += $$SHADOW_DIR/libs/xz/release/liblzma.a
else:win32:CONFIG(debug, debug|release): LIBS += $$SHADOW_DIR/libs/xz/debug/liblzma.a
else:unix: LIBS += $$SHADOW_DIR/libs/xz/liblzma.a
INCLUDEPATH += \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/rangecoder/ \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/simple/ \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/delta/ \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/common/ \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/check/ \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/lzma/ \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/api/ \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/lz/ \
    $$TOP_PWD/libs/xz/lzma/src/liblzma/ \
    $$TOP_PWD/libs/xz/lzma/src/common/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/xz/release/liblzma.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/xz/debug/liblzma.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/xz/release/lzma.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/xz/debug/lzma.lib
else:unix: PRE_TARGETDEPS += $$SHADOW_DIR/libs/xz/liblzma.a
