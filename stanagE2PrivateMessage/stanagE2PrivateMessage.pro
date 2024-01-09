# Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.

include(../config.pri)

QT += widgets

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = StanagE2PrivateMessagePlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \    
    StanagE2PrivateMessagePlugin.cxx \
    StanagE2PrivateMessageContentCreator.cxx \
    StanagE2PrivateMessageContent.cxx \
    StanagE2PrivateMessageControlWidgets.cxx \
    StanagE2PrivateMessagePolarityReceiver.cxx \
    StanagE2PrivateMessagePolaritySender.cxx 
HEADERS += \
    StanagE2PrivateMessagePlugin.h \
    StanagE2PrivateMessageContentCreator.h \
    StanagE2PrivateMessageContent.h \
    StanagE2PrivateMessageControlWidgets.h \
    StanagE2PrivateMessagePolarityReceiver.h \
    StanagE2PrivateMessagePolaritySender.h 

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include ../messageDefinitions/
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi -L$$OBJ_DIR -lGeneratedMessages

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target

