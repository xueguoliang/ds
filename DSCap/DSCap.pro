SOURCES += \
    main.cpp \
    ../DSUtil.cpp \
    DSApp.cpp \
    DSCap.cpp

HEADERS += \
    ../DSDef.h \
    ../DSUtil.h \
    DSApp.h \
    DSCap.h

QT += network multimedia widgets

win32{
INCLUDEPATH += C:\MinGW\msys\1.0\local\include
LIBS += -LC:\MinGW\msys\1.0\local\lib -lavdevice -lavfilter  \
-lpostproc -lavformat -lavcodec -lavutil -lswscale -lswresample

}

linux{
LIBS += -pthread -lavdevice -lavfilter -lswscale -lpostproc -lavformat -lavcodec -lxcb-xfixes -lxcb-render -lxcb-shape -lxcb -lX11 -lasound -lSDL -lx264 -lpthread -ldl -lfaac -lz -lswresample -lavutil -lm
}

mac{
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lavdevice -lavfilter -lswscale -lpostproc -lavformat -lavcodec -lavutil
}

RESOURCES += \
    res.qrc

linux:
{
    TARGET = DSCap.bin
}

linux:{
    LIBS += -lrt
}
