cmake_minimum_required(VERSION 3.27)
project(vcsi_sardinos)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Sardinos Plugin
include_directories(custom)

# MAVSDK
include_directories("C:/Users/dev/Documents/mavsdk/include")
include_directories("C:/Users/dev/Documents/mavsdk/include/mavsdk")
include_directories("C:/Users/dev/Documents/mavsdk/include/mavsdk/mavlink")
include_directories("C:/Users/dev/Documents/mavsdk/include/mavsdk/plugins/*")

# vcpkg
include_directories("C:/Users/dev/repos/vcpkg/installed/x64-windows/include")

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

# MOC
set(MOCS
    build/obj/moc_Drone.cpp
    build/obj/moc_ImageProcessor.cpp
    build/obj/moc_MissionDomain.cpp
    build/obj/moc_MissionPlanningContentCreator.cpp
    build/obj/moc_MissionPlanningDrawing.cpp
    build/obj/moc_MissionPlanningPlugin.cpp
    build/obj/moc_MissionPlanningWaypoint.cpp
    build/obj/moc_MissionPlanningWaypointConnector.cpp
    build/obj/moc_predefs.h
    build/obj/moc_Target.cpp
)

add_executable(vcsi_sardinos
        custom/BoundingBox.h
        custom/Drone.h
        custom/Drone.cpp
        custom/DroneWidget.h
        custom/ImageProcessor.h
        custom/ImageProcessor.cpp
        custom/MissionDomain.h
        custom/MissionDomain.cpp
        custom/MissionManager.h
        custom/MissionPlanningContentCreator.h
        custom/MissionPlanningContentCreator.cpp
        custom/MissionPlanningDrawing.h
        custom/MissionPlanningDrawing.cpp
        custom/MissionPlanningLine.h
        custom/MissionPlanningLine.cpp
        custom/MissionPlanningPlugin.h
        custom/MissionPlanningPlugin.cpp
        custom/MissionPlanningPolygon.h
        custom/MissionPlanningPolygon.cpp
        custom/MissionPlanningWaypoint.h
        custom/MissionPlanningWaypoint.cpp
        custom/MissionPlanningWaypointConnector.h
        custom/MissionPlanningWaypointConnector.cpp
        custom/Notifications.h
        custom/Notifications.cpp
        custom/Sardinos.h
        custom/Target.h
        custom/TargetWidget.h
        custom/UIHandler.h
        custom/UIHandler.cpp
)

