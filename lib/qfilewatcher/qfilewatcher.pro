#-------------------------------------------------
#
# Project created by QtCreator 2014-11-04T08:58:40
#
#-------------------------------------------------

QT       += core gui

TARGET = qfilewatcher
TEMPLATE = lib
CONFIG += staticlib

SOURCES += qfilewatcher.cpp \
    filemgr.cpp \
    filewatcher.cpp \
    filewatchercore.cpp \
    filewatcherimpl.cpp

HEADERS += qfilewatcher.h \
    filemgr.h \
    filewatcher.h \
    filewatchercore.h \
    filewatcherimpl.h \
    qsleeper.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
