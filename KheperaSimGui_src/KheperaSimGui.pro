#-------------------------------------------------
#
# Project created by QtCreator 2014-03-28T13:58:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KheperaSimGui
TEMPLATE = app


SOURCES += main.cpp\
        kheperasimgui.cpp \
    demo.cpp \
    control.cpp \
    remoteApi/extApiPlatform.c \
    remoteApi/extApiCustom.c \
    remoteApi/extApi.c

HEADERS  += kheperasimgui.h \
    i_control.h \
    include/v_repTypes.h \
    include/v_repLib.h \
    include/v_repConst.h \
    include/extApiCustomConst.h \
    remoteApi/extApiPlatform.h \
    remoteApi/extApiInternal.h \
    remoteApi/extApiCustom.h \
    demo.h \
    control.h

    remoteApi/extApi.h

FORMS    += kheperasimgui.ui

INCLUDEPATH += "include/" \
        "remoteApi/"
DEFINES += NON_MATLAB_PARSING \
        MAX_EXT_API_CONNECTIONS=255

QMAKE_CXXFLAGS += -std=c++11
