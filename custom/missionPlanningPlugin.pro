include(../config.pri)

QT += core widgets positioning gui

QMAKE_CXXFLAGS += /Z7 /FS /FC /MD
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_LFLAGS += /DEBUG /OPT:REF /OPT:NOICF
QMAKE_LFLAGS -= -O2
QMAKE_LFLAGS_RELEASE -= -O2
QMAKE_LFLAGS += /ignore:4099 /ignore:4229

unix:QMAKE_CXXFLAGS += -std=c++20 


TARGET = MissionPlanningPlugin
CONFIG += console c++1z
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    MissionPlanningPlugin.cpp \
    MissionPlanningContentCreator.cpp \
    FlightPather.cpp \
    MissionPlanningPolygon.cpp \
    MissionPlanningDrawing.cpp \
    MissionPlanningLine.cpp \
    MathExt.cpp \
    MissionPlanningWaypoint.cpp \
    MissionPlanningWaypointConnector.cpp \
    MissionDomain.cpp \
    Drone.cpp

HEADERS += \
    MissionPlanningPlugin.h \
    MissionPlanningContentCreator.h \
    FlightPather.h \
    SardinosPublisher.h \
    MissionPlanningPolygon.h \
    MissionPlanningDrawing.h \
    MissionPlanningLine.h \
    BoundingBox.h \
    MathExt.h \
    MissionPlanningWaypoint.h \
    MissionPlanningWaypointConnector.h \
    MissionDomain.h \
    Drone.h

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include c:\mavsdk\include c:\mavsdk\include\mavsdk
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi -lGroundControlStationDrawings_Mission -lc:\mavsdk\lib\mavsdk -lGroundControlStationDrawings_Track

release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target

RESOURCES += MissionPlanning.qrc