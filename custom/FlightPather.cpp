#include <FlightPather.h>
#include <qmath.h>

FlightPather::FlightPather(double turnRadius, double scanWidth)
    : turnRadius_(turnRadius), scanWidth_(scanWidth) {
        
}

FlightPather::~FlightPather(){}

double FlightPather::getDistance(QGeoCoordinate c1, QGeoCoordinate c2){
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