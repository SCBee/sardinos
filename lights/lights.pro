# Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
include(../config.pri)

QT += core widgets quick quickwidgets qml

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = LightsPluginExample
CONFIG += console
CONFIG += plugin
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += *.cxx
HEADERS += *.h

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include ../messageDefinitions/
INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../boost/include
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi -L$$OBJ_DIR -lGeneratedMessages

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target

include(service/lightsService.pri)
include(view/lightsView.pri)
