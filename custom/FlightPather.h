#pragma once

#include <QGeoCoordinate>

#include <BoundingBox.h>
#include <MissionPlanningWaypoint.h>

const double SCANWIDTHMETERS     = 500.0;
const double MAXDISTANCEMETERS   = 200000000.0;
const double TURNINGRADIUSMETERS = 200.0;

namespace sardinos::FlightPather
{
    QList<QGeoCoordinate> getVerticalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getHorizontalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getPath(BoundingBox missionBounds);
    bool canFly(QList<MissionPlanningWaypoint*> waypoints);
}  // namespace sardinos::FlightPather