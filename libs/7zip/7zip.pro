QT += core
TARGET = 7z
TEMPLATE = lib
CONFIG += \
    static \
    create_prl \
    link_prl \
    c++11

OTHER_FILES += $$PWD/7zip.pri
include(7zip-def.pri)

win32{
    include($$PWD/win/win.pri) #7zip
    QMAKE_CXXFLAGS += \
        -isystem C:/msys64/mingw64/include/QtGui \
        -isystem C:/msys64/mingw64/include/QtCore \
        -isystem C:/msys64/mingw64/include

}else{
    include($$PWD/unix/unix.pri) #p7zip
}

headers.files = $$PUBLIC_HEADERS
headers.path = $$INSTALL_HEADERS/$$TARGET
target.path = $$INSTALL_LIBS/

INSTALLS += \
    target \
    headers

