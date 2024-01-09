# Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.

include(../config.pri)

QT += widgets positioning

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = RoutesPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \    
    CloakingDevicePower.cxx \
    CustomCloakingDeviceUploadSource.cxx \
    CustomRoutePropertiesContent.cxx \
    CustomWaypointUploadContent.cxx \
    CustomWaypointUploadContentCreator.cxx \
    CustomWaypointEditingContent.cxx \
    DownloadedCloakingDevicePowerMessageReceiver.cxx \
    DownloadedCloakingDevicePowerView.cxx \
    DownloadedCloakingDevicePowerWaypointEditor.cxx \
    DownloadedWaypointCustomEditingContentCreator.cxx \
    DownloadedWaypointCustomEditingContent.cxx \
    PlannedWaypointCustomEditingContent.cxx \
    PlannedWaypointCloakingDeviceMutator.cxx \
    PlannedWaypointCustomEditingUi.cxx \
    RoutesPlugin.cxx \
    ReadonlyRoutesContentCreator.cxx \
    ReadonlyRoutesContent.cxx 
HEADERS += \
    CloakingDevicePower.h \
    CustomCloakingDeviceUploadSource.h \
    CustomRoutePropertiesContent.h \
    CustomWaypointUploadContent.h \
    CustomWaypointUploadContentCreator.h \
    CustomWaypointEditingContent.h \
    DownloadedCloakingDevicePowerMessageReceiver.h \
    DownloadedCloakingDevicePowerView.h \
    DownloadedCloakingDevicePowerWaypointEditor.h \
    DownloadedWaypointCustomEditingContentCreator.h \
    DownloadedWaypointCustomEditingContent.h \
    PlannedWaypointCustomEditingContent.h \
    PlannedWaypointCloakingDeviceMutator.h \
    PlannedWaypointCustomEditingUi.h \
    RoutesPlugin.h \
    ReadonlyRoutesContentCreator.h \
    ReadonlyRoutesContent.h 

FORMS += \
    PlannedWaypointCustomProperties.ui


INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include ../messageDefinitions/
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi -L$$OBJ_DIR -lGeneratedMessages

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target
