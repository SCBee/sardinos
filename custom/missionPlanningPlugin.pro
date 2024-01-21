include(../config.pri)

QT += core widgets positioning gui

unix:QMAKE_CXXFLAGS += -std=c++20

TARGET = MissionPlanningPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    FlightPather.cpp \
    MissionPlanningPlugin.cpp \
    MissionPlanningContentCreator.cpp \
    MissionPlanningPolygon.cpp \
    MissionPlanningDrawing.cpp \
    MissionPlanningLine.cpp \
    MathExt.cpp \
    MissionPlanningWaypoint.cpp \
    MissionplanningWaypointConnector.cpp \
    MissionDomain.cpp

HEADERS += \
    FlightPather.h \
    MissionPlanningPlugin.h \
    MissionPlanningContentCreator.h \
    MissionPlanningPolygon.h \
    MissionPlanningDrawing.h \
    MissionPlanningLine.h \
    BoundingBox.h \
    MathExt.h \
    MissionPlanningWaypoint.h \
    MissionplanningWaypointConnector.h \
    MissionDomain.h



INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include 
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi -lGroundControlStationDrawings_Mission

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target

RESOURCES += MissionPlanning.qrc