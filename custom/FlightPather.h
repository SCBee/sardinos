#pragma once

#include <QGeoCoordinate>

#include <BoundingBox.h>
#include <MissionPlanningWaypoint.h>
#include <MissionPlanningWaypointConnector.h>

class FlightPather
{
public:
    FlightPather(double turnRadiusMeters,
                 double scanWidthMeters,
                 double maxDistanceMeters);
    ~FlightPather();

    double getDistance(QGeoCoordinate c1, QGeoCoordinate c2);
    QList<MissionPlanningWaypoint> getVerticalFlightPath(BoundingBox missionBounds);
    QList<MissionPlanningWaypoint> getHorizontalFlightPath(BoundingBox missionBounds);
    bool canFly(BoundingBox missionBounds);
    std::pair<QList<MissionPlanningWaypoint>,
              QList<MissionPlanningWaypointConnector>>
    path();

private:
    double turnRadiusMeters_;
    double scanWidthMeters_;
    double maxDistanceMeters_;

    QList<MissionPlanningWaypoint> waypoints_;
    QList<MissionPlanningWaypointConnector> connectors_;
};