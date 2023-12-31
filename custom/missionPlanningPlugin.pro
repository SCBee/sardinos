include(../config.pri)

QT += core widgets positioning

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = MissionPlanningPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    MissionPlanningPlugin.cpp \
    MissionPlanningContentCreator.cpp 
HEADERS += \
    MissionPlanningPlugin.h \
    MissionPlanningContentCreator.h 

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include 
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target

RESOURCES += MissionPlanning.qrc