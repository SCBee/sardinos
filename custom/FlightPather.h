#pragma once

#include <QGeoCoordinate>

class FlightPather {

public:
    FlightPather(double turnRadius, double scanWidth);
    ~FlightPather();

    double getDistance(QGeoCoordinate c1, QGeoCoordinate c2);

private:
    double turnRadius_;
    double scanWidth_;
};