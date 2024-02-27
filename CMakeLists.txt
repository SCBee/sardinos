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

# MOC
set(MOCS
        build/obj/moc_predefs.h
        build/obj/moc_ContentCreator.cpp
        build/obj/moc_Drawing.cpp
        build/obj/moc_Drone.cpp
        build/obj/moc_ImageProcessor.cpp
        build/obj/moc_MissionDomain.cpp
        build/obj/moc_MissionPlanningPlugin.cpp
        build/obj/moc_Target.cpp
        build/obj/moc_Waypoint.cpp
        build/obj/moc_DroneTelemetry.cpp
        build/obj/moc_WaypointConnector.cpp
)

add_executable(vcsi_sardinos
        custom/Drone/Drone.cpp
        custom/Drone/Drone.h
        custom/Drone/DroneTelemetry.cpp
        custom/Drone/DroneTelemetry.h
        custom/Drone/DroneWidget.h
        custom/Drone/MissionManager.h

        custom/Helpers/BoundingBox.h
        custom/Helpers/Sardinos.h

        custom/Image/ImageProcessor.cpp
        custom/Image/ImageProcessor.h
        custom/Image/Target.h
        custom/Image/TargetWidget.h

        custom/UI/Drawing.cpp
        custom/UI/Drawing.h
        custom/UI/Line.cpp
        custom/UI/Line.h
        custom/UI/Notifications.cpp
        custom/UI/Notifications.h
        custom/UI/Polygon.cpp
        custom/UI/Polygon.h
        custom/UI/UIHandler.cpp
        custom/UI/UIHandler.h

        custom/Waypoint/MissionDomain.cpp
        custom/Waypoint/MissionDomain.h
        custom/Waypoint/Waypoint.cpp
        custom/Waypoint/Waypoint.h
        custom/Waypoint/WaypointConnector.cpp
        custom/Waypoint/WaypointConnector.h

        custom/ContentCreator.cpp
        custom/ContentCreator.h
        custom/MissionPlanningPlugin.cpp
        custom/MissionPlanningPlugin.h
        ${MOCS}
)

