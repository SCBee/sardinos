#pragma once

#include <QGeoCoordinate>


const double ALTITUDEMETERS = 1300;

struct BoundingBox
{
public:
    QGeoCoordinate SW;
    QGeoCoordinate NW;
    QGeoCoordinate SE;
    QGeoCoordinate NE;

    QList<QGeoCoordinate> list()
    {
        SW.setAltitude(ALTITUDEMETERS);
        SE.setAltitude(ALTITUDEMETERS);
        NE.setAltitude(ALTITUDEMETERS);
        NW.setAltitude(ALTITUDEMETERS);

        return {SW, SE, NE, NW};
    }

    double eastBound() { return NE.longitude(); }

    double northBound() { return NE.latitude(); }

    double westBound() { return SW.longitude(); }

    double southBound() { return SW.latitude(); }
};