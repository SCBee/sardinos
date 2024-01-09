# Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.

include(../config.pri)

QT += widgets positioning quick qml

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = GraphicsWidgetPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    GdtContent.cxx \
    GdtWidgets.cxx \
    GraphicsWidgetContentCreator.cxx \
    GraphicsWidgetPlugin.cxx \
    VehicleGraphicsWidgetDisplay.cxx \
    QmlGdtView.cxx \

HEADERS += \
    GdtContent.h \
    GdtWidgets.h \
    GraphicsWidgetContentCreator.h \
    GraphicsWidgetPlugin.h \
    VehicleGraphicsWidgetDisplay.h \
    QmlGdtView.h \

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi -L$$OBJ_DIR

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target
