#-------------------------------------------------
#
# Project created by QtCreator 2010-07-14T17:54:25
#
#-------------------------------------------------
#if QT_VERSION >= 0x050000
QT       += core gui widgets
#else
QT       += core gui
#endif

TARGET = qcom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        qextserial/qextserialport.cpp \
    dlt645_2007/dlt645_api_07.c \
    dlt645_2007/dlt645_api_test.c

HEADERS  += mainwindow.h \
        qextserial/qextserialport_global.h \
        qextserial/qextserialport.h \
    dlt645_2007/dlt645_api_07.h

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
RC_FILE += myico.rc
