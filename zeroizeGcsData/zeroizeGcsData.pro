# Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.

include(../config.pri)

QT += concurrent

unix:{
   QMAKE_CXXFLAGS += -std=c++20
   QMAKE_LFLAGS += -Wl,-rpath,../lib/plugins/vcsi
}                          

win32:{
   DEFINES += LMCDL_IMPORT_SHARED
   INCLUDEPATH += .
   LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi
}

TARGET = ZeroizeGcsDataExample
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += *.cxx
HEADERS += *.h

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lZeroizePlugin

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target
