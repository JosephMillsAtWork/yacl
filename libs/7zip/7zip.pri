win32:CONFIG(release, debug|release): LIBS += $$SHADOW_DIR/libs/7zip/release/lib7z.a
else:win32:CONFIG(debug, debug|release): LIBS += $$SHADOW_DIR/libs/7zip/debug/lib7z.a
else:unix: LIBS += $$SHADOW_DIR/libs/7zip/lib7z.a
##FIXME add includess and deps
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/7zip/release/lib7z.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/7zip/debug/lib7z.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/7zip/release/7z.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$SHADOW_DIR/libs/7zip/debug/7z.lib
else:unix: PRE_TARGETDEPS += $$SHADOW_DIR/libs/7zip/lib7z.a

