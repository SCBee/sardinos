# Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
include(../config.pri)

QT += widgets

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = JoystickPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    JoystickDisplay.cxx \
    JoystickPlugin.cxx
HEADERS += \
    JoystickDisplay.h \
    JoystickPlugin.h

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target
