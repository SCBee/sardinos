#pragma once

#include <QGeoCoordinate>
#include <qmath.h>

struct BoundingBox {
    public:
        QGeoCoordinate SW;
        QGeoCoordinate NW;
        QGeoCoordinate SE;
        QGeoCoordinate NE;

        QList<QGeoCoordinate> list(){
            return { SW, SE, NE, NW };
        }

        double eastBound(){
            return NE.longitude();
        }

        double northBound(){
            return NE.latitude();
        }

        double westBound(){
            return SW.longitude();
        }

        double southBound(){
            return SW.latitude();
        }
        
        bool isVertical(){
            if (getDistance(SE, SW) > getDistance(SE, NE)) return false;
            return true;
        }

    private:
        double getDistance(QGeoCoordinate c1, QGeoCoordinate c2){
            const double R = 6378.137;
            double dLat = (c2.latitude() * M_PI / 180) - (c1.latitude() * M_PI / 180);
            double dLon = (c2.longitude() * M_PI / 180) - (c1.longitude() * M_PI / 180);
            double a = sin(dLat / 2) * sin(dLat / 2) +
                    cos(c1.latitude() * M_PI / 180) * cos(c2.latitude() * M_PI / 180) *
                    sin(dLon / 2) * sin(dLon / 2);
            double c = 2 * atan2(sqrt(a), sqrt(1 - a));
            double d = R * c;
            return d * 1000; 
}

};