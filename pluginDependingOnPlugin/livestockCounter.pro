# Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
include(../config.pri)

QT += widgets positioning

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = LivestockCounterPlugin
CONFIG += console
CONFIG -= app_bundle
      
TEMPLATE = lib

SOURCES += \
	 LivestockCounterContent.cxx \
	 LivestockCounterPlugin.cxx \
HEADERS += \
	 LivestockCounterContent.h \
    LivestockCounterPlugin.h \
	 I_AnimalTrackerApi.h \

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target
