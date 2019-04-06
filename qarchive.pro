TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    $$PWD/libs/libs.pro \
    $$PWD/examples/examples.pro
## include($$PWD/deployment/deployment.pri)

OTHER_FILES += \
    $$PWD/README.md \
    $$PWD/.qmake.conf \
    $$PWD/.gitignore
