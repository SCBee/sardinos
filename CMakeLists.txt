cmake_minimum_required(VERSION 3.27)
project(vcsi_sardinos)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


include_directories("C:/Users/dev/Documents/mavsdk/include")
include_directories("C:/Users/dev/Documents/mavsdk/include/mavsdk")
include_directories("C:/Users/dev/Documents/mavsdk/include/mavsdk/mavlink")
include_directories("C:/Users/dev/Documents/mavsdk/include/mavsdk/plugins/*")
include_directories(custom)

add_executable(vcsi_sardinos
        custom/BoundingBox.h
        custom/Drone.cpp
        custom/Drone.h
        custom/DroneWidget.h
        custom/FlightPather.cpp
        custom/FlightPather.h
        custom/MathExt.cpp
        custom/MathExt.h
        custom/MissionDomain.cpp
        custom/MissionDomain.h
        custom/MissionPlanningContentCreator.cpp
        custom/MissionPlanningContentCreator.h
        custom/MissionPlanningDrawing.cpp
        custom/MissionPlanningDrawing.h
        custom/MissionPlanningLine.cpp
        custom/MissionPlanningLine.h
        custom/MissionPlanningPlugin.cpp
        custom/MissionPlanningPlugin.h
        custom/MissionPlanningPolygon.cpp
        custom/MissionPlanningPolygon.h
        custom/MissionPlanningWaypoint.cpp
        custom/MissionPlanningWaypoint.h
        custom/MissionPlanningWaypointConnector.cpp
        custom/MissionPlanningWaypointConnector.h
        custom/SardinosPublisher.h
)


# LMT and QT Base
include_directories(.)
include_directories("C:/Program Files/LMCDL/vcsi/sdk/include")
include_directories("C:/Qt/Qt5.12.7/5.12.7/msvc2015_64/bin")
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include")

# QT Specific (msvc)
include_directories("C:/Qt/Qt5.12.7/5.12.7/msvc2015_64/include/QtCore")
include_directories("C:/Qt/Qt5.12.7/5.12.7/msvc2015_64/include")
include_directories("C:/Qt/Qt5.12.7/5.12.7/msvc2015_64/include/QtPositioning")
include_directories("C:/Qt/Qt5.12.7/5.12.7/msvc2015_64/include/QtGui")
include_directories("C:/Qt/Qt5.12.7/5.12.7/msvc2015_64/include/QtQml")
include_directories("C:/Qt/Qt5.12.7/5.12.7/msvc2015_64/include/QtQuick")
include_directories("C:/Qt/Qt5.12.7/5.12.7/msvc2015_64/include/QtWidgets")

# QT Specific (mingw)
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include/QtCore")
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include")
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include/QtPositioning")
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include/QtGui")
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include/QtQml")
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include/QtQuick")
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include/QtWidgets")
include_directories("C:/Qt/Qt5.12.7/5.12.7/mingw73_64/include/QtPositioning/5.12.7/QtPositioning/private")

# custom libs
include_directories("C:/Program Files/LMCDL/vcsi/sdk/examples/custom")