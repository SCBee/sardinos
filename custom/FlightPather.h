#pragma once

#include <QGeoCoordinate>

#include <BoundingBox.h>
#include <MissionPlanningWaypoint.h>

const auto SCANWIDTHMETERS = 500;
const auto MAXDISTANCEMETERS = 200000000;
const auto TURNINGRADIUSMETERS = 200;

class FlightPather
{
public:
    FlightPather();
    ~FlightPather();

    QList<QGeoCoordinate> getVerticalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getHorizontalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getPath(BoundingBox missionBounds);
    bool canFly(QList<MissionPlanningWaypoint*> waypoints);
};