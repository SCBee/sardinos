#pragma once

#include <QGeoCoordinate>

#include <BoundingBox.h>
#include <MissionPlanningWaypoint.h>

const double SCANWIDTHMETERS     = 500.0;
const double MAXDISTANCEMETERS   = 200000000.0;
const double TURNINGRADIUSMETERS = 200.0;

namespace sardinos::FlightPather
{
    QList<QGeoCoordinate> getVerticalFlightPath(
        const BoundingBox& missionBounds);
    QList<QGeoCoordinate> getHorizontalFlightPath(
        const BoundingBox& missionBounds);
    QList<QGeoCoordinate> getPath(const BoundingBox& missionBounds);
    bool canFly(const QList<MissionPlanningWaypoint*>& waypoints);
}  // namespace sardinos::FlightPather