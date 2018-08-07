HEADERS += \
    ../DSDef.h \
    DSApp.h \
    DSSetup.h \
    ../DSUtil.h \
    ../DSAbout.h \
    ../DSLogin.h \
    ../DSRegister.h

SOURCES += \
    DSApp.cpp \
    DSSetup.cpp \
    ../DSUtil.cpp \
    ../DSAbout.cpp \
    ../DSLogin.cpp \
    ../DSRegister.cpp

QT += widgets gui network

FORMS +=

RESOURCES += \
    res.qrc

win32:{
RC_ICONS = logo.ico
RC_FILE = myapp.rc
}


mac:{
    ICON = logo.icns
}

linux:
{
    TARGET = DS.bin
}

linux:{
    LIBS += -lrt
}



