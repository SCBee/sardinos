# Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
include(../config.pri)

QT += core

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = GeneratedMessages
CONFIG += staticlib
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    Message44.cxx \
    Message107.cxx \
    Message1302.cxx \
    Message1303.cxx \
    Message30264.cxx \
    Message30265.cxx \
    Message33264.cxx \
    Message33265.cxx
HEADERS += \
    Message44.h \
    Message107.h \
    Message1302.h \
    Message1303.h \
    Message30264.h \
    Message30265.h \
    Message33264.h \
    Message33265.h

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include 
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi 

release:DESTDIR = $$OBJ_DIR
debug:DESTDIR = $$OBJ_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

