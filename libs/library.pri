TEMPLATE = lib
CONFIG += static \
        create_prl \
        link_prl
MOC_DIR = $$OUT_PWD/../$$TARGET-moc
OBJECTS_DIR = $$OUT_PWD/../$$TARGET-moc
RCC_DIR = $$OUT_PWD/../$$TARGET-moc

TR_EXCLUDE += $$PWD/$$TARGET/*
SRC_DIR=$$PWD/$$TARGET/$$TARGET/
PRO_DIR=$$PWD/$$TARGET/
