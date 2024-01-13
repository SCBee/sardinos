#pragma once

#include <QGeoCoordinate>
#include <BoundingBox.h>

class FlightPather {

public:
    FlightPather(double turnRadiusMeters, double scanWidthMeters);
    ~FlightPather();

    double getDistance(QGeoCoordinate c1, QGeoCoordinate c2);
    QList<QGeoCoordinate> getFlightPath(BoundingBox missionBounds);

private:
    double turnRadiusMeters_;
    double scanWidthMeters_;
};