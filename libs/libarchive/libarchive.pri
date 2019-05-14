osx{
    LIBS += -framework CoreServices -liconv
}

win32:CONFIG(release, debug|release): LIBS += $$SHADOW_DIR/libs/libarchive/release/libarchive.a
else:win32:CONFIG(debug, debug|release): LIBS += $$SHADOW_DIR/libs/libarchive/debug/libarchive.a
else:unix: LIBS += $$SHADOW_DIR/libs/libarchive/libarchive.a

INCLUDEPATH += $$TOP_PWD/libs/libarchive/libarchive/libarchive
DEPENDPATH += $$TOP_PWD/libs/libarchive/libarchive/libarchive

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/libarchive/release/libarchive.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/libarchive/debug/libarchive.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/libarchive/release/archive.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/libarchive/debug/archive.lib
else:unix: PRE_TARGETDEPS += $$SHADOW_DIR/libs/libarchive/libarchive.a

