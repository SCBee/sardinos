#pragma once

#include <QGeoCoordinate>
#include <BoundingBox.h>
#include <MissionPlanningWaypoint.h>

class FlightPather {

public:
    FlightPather(double turnRadiusMeters, double scanWidthMeters, double maxDistanceMeters);
    ~FlightPather();

    double getDistance(QGeoCoordinate c1, QGeoCoordinate c2);
    QList<QGeoCoordinate> getVerticalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getHorizontalFlightPath(BoundingBox missionBounds);
    bool canFly(BoundingBox missionBounds);
    QList<MissionPlanningWaypoint> path();

private:
    double turnRadiusMeters_;
    double scanWidthMeters_;
    double maxDistanceMeters_;

    QList<MissionPlanningWaypoint> lines_ = QList<MissionPlanningWaypoint>();
};