#pragma once

#include <QGeoCoordinate>

#include <BoundingBox.h>
#include <MissionPlanningWaypoint.h>

const auto SCANWIDTHMETERS = 500;
const auto MAXDISTANCEMETERS = 2000;
const auto TURNINGRADIUSMETERS = 200;

class FlightPather
{
public:
    FlightPather();
    ~FlightPather();

    double getDistance(QGeoCoordinate c1, QGeoCoordinate c2);
    QList<QGeoCoordinate> getVerticalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getHorizontalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getPath(BoundingBox missionBounds);
    bool canFly(QList<MissionPlanningWaypoint*> waypoints);
};