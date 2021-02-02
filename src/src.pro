#-------------------------------------------------
#
# Project created by QtCreator 2020-03-11T12:52:52
#
#-------------------------------------------------

QT       += core gui multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lanCommunicator
TEMPLATE = app
CONFIG += c++11

#uncomment from static build
#QMAKE_LFLAGS_RELEASE += -static -static-libgcc

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,,d)
#    unix: TARGET = $${TARGET}_debug
#    else: TARGET = $${TARGET}d
}

OBJECTS_DIR         = ../build/obj/$${CONFIGURATION}
MOC_DIR             = ../build/$${CONFIGURATION}
RCC_DIR             = ../build/rcc
UI_DIR              = ../build/ui
DESTDIR             = ../bin

win32|win64{
    RC_FILE=  index.rc
    OTHER_FILES+= index.rc
    DISTFILES += index.rc
}
QMAKE_CXXFLAGS += "-std=c++11"

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    searcher.cpp \
    global.cpp \
    windows/username.cpp \
    widgets/tabwidget.cpp \
    windows/contacteditor.cpp

HEADERS += \
        mainwindow.h \
    searcher.h \
    global.h \
    version.h \
    windows/username.h \
    widgets/tabwidget.h \
    windows/contacteditor.h

FORMS += \
        mainwindow.ui \
    widgets/tabwidget.ui \
    windows/contacteditor.ui

TRANSLATIONS = lang/ru_RU.ts


exists(./gitversion.pri):include(./gitversion.pri)
exists(./myLibs.pri):include(./myLibs.pri)

DISTFILES += \
    gitversion.pri \
    index.rc \
    myLibs.pri

RESOURCES += \
    resources.qrc
