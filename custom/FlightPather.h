#pragma once

#include <QGeoCoordinate>
#include <BoundingBox.h>
#include <MissionPlanningLine.h>

class FlightPather {

public:
    FlightPather(double turnRadiusMeters, double scanWidthMeters, double maxDistanceMeters);
    ~FlightPather();

    double getDistance(QGeoCoordinate c1, QGeoCoordinate c2);
    QList<QGeoCoordinate> getVerticalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getHorizontalFlightPath(BoundingBox missionBounds);
    QList<MissionPlanningLine*> getFlightPath(BoundingBox missionBounds);

private:
    double turnRadiusMeters_;
    double scanWidthMeters_;
    double maxDistanceMeters_;
};