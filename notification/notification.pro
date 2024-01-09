# Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.

include(../config.pri)

QT += widgets

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = NotificationPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    NotificationPlugin.cxx \
    NotificationContentCreator.cxx \
    ExampleNotificationContent.cxx \
    NotificationWithLongString.cxx \
    NotificationToBeRemovedAfterTime.cxx

HEADERS += \
    NotificationPlugin.h \
    NotificationContentCreator.h \
    ExampleNotificationContent.h \
    NotificationWithLongString.h \
    NotificationToBeRemovedAfterTime.h

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi -L$$OBJ_DIR

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target
