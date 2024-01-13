#include <FlightPather.h>
#include <qmath.h>

FlightPather::FlightPather(double turnRadiusMeters, double scanWidthMeters)
    : turnRadiusMeters_(turnRadiusMeters), scanWidthMeters_(scanWidthMeters) {
        
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

QList<QGeoCoordinate> FlightPather::getFlightPath(BoundingBox missionBounds){

    auto longSpreadMeters = getDistance(missionBounds.NE, missionBounds.NW);

    auto longSpread = abs(missionBounds.NE.longitude() - missionBounds.NW.longitude());

    auto steps = longSpreadMeters / scanWidthMeters_;

    auto stepLongitude = longSpread / steps;

    auto currentLocation = missionBounds.SW;

    currentLocation.setLongitude(currentLocation.longitude() + (stepLongitude / 2.0));

    auto wayPoints = QList<QGeoCoordinate>();
    
    auto goingUp = true;

    while (currentLocation.longitude() < missionBounds.NE.longitude()){
        wayPoints.append(currentLocation);

        if (goingUp && currentLocation.latitude() == missionBounds.SW.latitude()){
            currentLocation.setLatitude(missionBounds.NW.latitude());
            continue;
        }

        if (goingUp && currentLocation.latitude() == missionBounds.NW.latitude()){
            currentLocation.setLongitude(currentLocation.longitude() + stepLongitude);
            goingUp = false;
            continue;
        }

        if (!goingUp && currentLocation.latitude() == missionBounds.NW.latitude()){
            currentLocation.setLatitude(missionBounds.SW.latitude());
            continue;
        }

        if (!goingUp && currentLocation.latitude() == missionBounds.SW.latitude()){
            currentLocation.setLongitude(currentLocation.longitude() + stepLongitude);
            goingUp = true;
            continue;
        }
    }
}