# Copyright (c) 2017 Lockheed Martin Canada Corp. All rights reserved.
include(../config.pri)

QT += widgets positioning

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = MapDrawingPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    MapDrawingPlugin.cxx \
    FaaObstacleContent.cxx \
    FaaObstacleSource.cxx
HEADERS += \
    MapDrawingPlugin.h \  
    FaaObstacleContent.h \
    FaaObstacleSource.h \
    FaaObstacleInfo.h
FORMS += \
    FaaObstacleForm.ui

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target
