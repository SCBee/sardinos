#pragma once

#include <QGeoCoordinate>
#include <BoundingBox.h>

class FlightPather {

public:
    FlightPather(double turnRadiusMeters, double scanWidthMeters);
    FlightPather(){turnRadiusMeters_ = 0; scanWidthMeters_ = 0;};
    ~FlightPather();

    double getDistance(QGeoCoordinate c1, QGeoCoordinate c2);
    QList<QGeoCoordinate> getVerticalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getHorizontalFlightPath(BoundingBox missionBounds);

private:
    double turnRadiusMeters_;
    double scanWidthMeters_;
};