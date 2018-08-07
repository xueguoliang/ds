
HEADERS += \
    DSHttpServer.h \
    DSHttpsServer.h \
    DSDBConnector.h
SOURCES += \
    main.cpp \
    ../DSUtil.cpp \
    DSHttpServer.cpp \
    DSHttpsServer.cpp \
    DSDBConnector.cpp

linux:{
TARGET = DSServer.bin
}

CONFIG += C++11 TUFAO1
QT += sql

RESOURCES += \
    res.qrc

LIBS += -lrt
