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
    bool canFly(BoundingBox missionBounds);
    QList<MissionPlanningLine*> path();

private:
    double turnRadiusMeters_;
    double scanWidthMeters_;
    double maxDistanceMeters_;

    QList<MissionPlanningLine*> lines_ = QList<MissionPlanningLine*>();
};