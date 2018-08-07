SOURCES += \
    main.cpp \
    DSView.cpp \
    DSRecv.cpp \
    ../DSAbout.cpp \
    ../DSUtil.cpp \
    ../DSLogin.cpp \
    ../DSRegister.cpp

HEADERS += \
    DSView.h \
    DSRecv.h \
    ../DSAbout.h \
    ../DSUtil.h \
    ../DSLogin.h \
    ../DSRegister.h

QT += widgets gui network

RESOURCES += \
    res.qrc

win32:{
RC_ICONS = logo.ico
RC_FILE = myapp.rc
}

mac:{
    ICON = logo.icns
}

linux:{
    LIBS += -lrt
}

linux:
{
    TARGET = DSView.bin
}


