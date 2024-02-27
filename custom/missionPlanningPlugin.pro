include(../config.pri)

QT += core widgets positioning gui

QMAKE_CXXFLAGS += /Z7 /FS /FC /MD /wd4005
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

SOURCES += *.cpp \
           Drone/*.cpp \
           Image/*.cpp \
           UI/*.cpp \
           Waypoint/*.cpp

HEADERS += *.h \
           Drone/*.h \
           Helpers/*.h \
           Image/*.h \
           UI/*.h \
           Waypoint/*.h

INCLUDEPATH += $$EXAMPLES_ROOT_DIRECTORY/../include C:\Users\luisc\Documents\mavsdk-windows-x64-release\include C:\Users\luisc\Documents\mavsdk-windows-x64-release\include\mavsdk
win32:INCLUDEPATH += .
LIBS += -L$$EXAMPLES_ROOT_DIRECTORY/../lib -lvcsiapi -lGroundControlStationDrawings_Mission -lC:\Users\luisc\Documents\mavsdk-windows-x64-release\lib\mavsdk -lGroundControlStationDrawings_Track

INCLUDEPATH += "C:\Program Files\LMCDL\vcsi\sdk\include" \
                C:\Users\luisc\Documents\mavsdk-windows-x64-release\include\
                C:\Users\luisc\Documents\mavsdk-windows-x64-release\include\mavsdk \
                C:\Users\luisc\Documents\x64-windows\include

LIBS += -L"C:\Program Files\LMCDL\vcsi\sdk\lib" \
        -lvcsiapi \
        -lGroundControlStationDrawings_Mission \
        -lC:\Users\luisc\Documents\mavsdk-windows-x64-release\lib\mavsdk \
        -lGroundControlStationDrawings_Track

LIBS += -L"C:\Users\luisc\Documents\x64-windows\lib" \
        -lopencv_core4 \
        -lopencv_imgproc4 \
        -lopencv_imgcodecs4 \
        -lopencv_highgui4 \
        -lopencv_videoio4

LIBS += -L"C:\Users\luisc\Documents\x64-windows\bin" \
        -lopencv_core4 \
        -lopencv_imgproc4 \
        -lopencv_imgcodecs4 \
        -lopencv_highgui4 \
        -lopencv_videoio4


release:DESTDIR = $$BUILD_DIR
debug:DESTDIR = $$BUILD_DIR
OBJECTS_DIR = $$OBJ_DIR
MOC_DIR = $$OBJ_DIR

target.path = $$PLUGIN_DIR
INSTALLS += target

RESOURCES += MissionPlanning.qrc

