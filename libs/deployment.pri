isEmpty(INSTALL_PREFIX): INSTALL_PREFIX=$$OUT_PWD/usr

isEmpty(PUBLIC_HEADERS): message( WARN: there are no public headers to install )
headers.files = $$PUBLIC_HEADERS
headers.CONFIG += no_check_exist
headers.path = $$INSTALL_HEADERS
target.path = $$INSTALL_LIBS
INSTALLS += \
    target \
    headers
