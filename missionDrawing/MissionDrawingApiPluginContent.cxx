/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <MissionDrawingApiPluginContent.h>

#include <ExampleGeospatialSimpleWaypoint.h>
#include <LmCdl/I_MissionDrawingApi.h>
#include <QList>

MissionDrawingApiPluginContent::MissionDrawingApiPluginContent(LmCdl::I_MissionDrawingApi& missionDrawingApi)
    : missionDrawingApi_(missionDrawingApi)
    , domain_()
{
    foreach (ExampleGeospatialSimpleWaypoint* waypoint, domain_.waypoints()) {
        missionDrawingApi_.addDrawingForWaypoint(*waypoint);
    }
    foreach (LmCdl::I_SimpleWaypointConnector* waypointConnector, domain_.waypointConnectors()) {
        missionDrawingApi_.addDrawingForWaypointConnector(*waypointConnector);
    }
}

MissionDrawingApiPluginContent::~MissionDrawingApiPluginContent()
{
    foreach (LmCdl::I_SimpleWaypointConnector* waypointConnector, domain_.waypointConnectors()) {
        missionDrawingApi_.removeDrawingForWaypointConnector(*waypointConnector);
    }
    foreach (ExampleGeospatialSimpleWaypoint* waypoint, domain_.waypoints()) {
        missionDrawingApi_.removeDrawingForWaypoint(*waypoint);
    }
}
