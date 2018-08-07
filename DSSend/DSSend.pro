SOURCES += \
    main.cpp \
    DSApp.cpp \
    ../DSUtil.cpp \
    DSFrame.cpp \
    DSSend.cpp

HEADERS += \
    DSApp.h \
    ../DSDef.h \
    ../DSUtil.h \
    DSFrame.h \
    DSSend.h

QT += network

RESOURCES += \
    res.qrc

linux:
{
    TARGET = DSSend.bin
}

linux:{
    LIBS += -lrt
}
