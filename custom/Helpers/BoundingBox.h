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

    [[nodiscard]] double eastBound() const { return NE.longitude(); }

    [[nodiscard]] double northBound() const { return NE.latitude(); }

    [[nodiscard]] double westBound() const { return SW.longitude(); }

    [[nodiscard]] double southBound() const { return SW.latitude(); }
};