# Copyright (c) 2019 Lockheed Martin Canada Inc. All rights reserved.
include(../config.pri)

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = ShapesPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    ShapesPlugin.cxx
HEADERS += \
    ShapesPlugin.h

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target
