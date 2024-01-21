#include <FlightPather.h>
#include <qmath.h>

FlightPather::FlightPather(double turnRadiusMeters, double scanWidthMeters, double maxDistanceMeters)
    : turnRadiusMeters_(turnRadiusMeters), scanWidthMeters_(scanWidthMeters), maxDistanceMeters_(maxDistanceMeters)
{
}

FlightPather::~FlightPather() {}

double FlightPather::getDistance(QGeoCoordinate c1, QGeoCoordinate c2)
{
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

QList<QGeoCoordinate> FlightPather::getVerticalFlightPath(BoundingBox missionBounds)
{

    auto longSpreadMeters = getDistance(missionBounds.NE, missionBounds.NW);

    auto longSpread = abs(missionBounds.eastBound() - missionBounds.westBound());

    auto steps = longSpreadMeters / scanWidthMeters_;

    auto stepLongitude = longSpread / steps;

    auto currentLocation = missionBounds.SW;

    currentLocation.setLongitude(currentLocation.longitude() + (stepLongitude / 2.0));

    auto wayPoints = QList<QGeoCoordinate>();

    auto goingUp = true;

    while (currentLocation.longitude() < missionBounds.eastBound())
    {
        wayPoints.append(currentLocation);

        if (goingUp && currentLocation.latitude() == missionBounds.southBound())
        {
            currentLocation.setLatitude(missionBounds.northBound());
            continue;
        }

        if (goingUp && currentLocation.latitude() == missionBounds.northBound())
        {
            currentLocation.setLongitude(currentLocation.longitude() + stepLongitude);
            goingUp = false;
            continue;
        }

        if (!goingUp && currentLocation.latitude() == missionBounds.northBound())
        {
            currentLocation.setLatitude(missionBounds.southBound());
            continue;
        }

        if (!goingUp && currentLocation.latitude() == missionBounds.southBound())
        {
            currentLocation.setLongitude(currentLocation.longitude() + stepLongitude);
            goingUp = true;
            continue;
        }
    }

    return wayPoints;
}

QList<QGeoCoordinate> FlightPather::getHorizontalFlightPath(BoundingBox missionBounds)
{

    auto latSpreadMeters = getDistance(missionBounds.NE, missionBounds.SE);

    auto latSpread = abs(missionBounds.northBound() - missionBounds.southBound());

    auto steps = latSpreadMeters / scanWidthMeters_;

    auto stepLatitude = latSpread / steps;

    auto currentLocation = missionBounds.SW;

    currentLocation.setLatitude(currentLocation.latitude() + (stepLatitude / 2.0));

    auto wayPoints = QList<QGeoCoordinate>();

    auto goingRight = true;

    while (currentLocation.latitude() < missionBounds.northBound())
    {
        wayPoints.append(currentLocation);

        if (goingRight && currentLocation.longitude() == missionBounds.westBound())
        {
            currentLocation.setLongitude(missionBounds.eastBound());
            continue;
        }

        if (goingRight && currentLocation.longitude() == missionBounds.eastBound())
        {
            currentLocation.setLatitude(currentLocation.latitude() + stepLatitude);
            goingRight = false;
            continue;
        }

        if (!goingRight && currentLocation.longitude() == missionBounds.eastBound())
        {
            currentLocation.setLongitude(missionBounds.westBound());
            continue;
        }

        if (!goingRight && currentLocation.longitude() == missionBounds.westBound())
        {
            currentLocation.setLatitude(currentLocation.latitude() + stepLatitude);
            goingRight = true;
            continue;
        }
    }

    return wayPoints;
}

bool FlightPather::canFly(BoundingBox missionBounds)
{
    auto distance = 0.0;

    if (missionBounds.isVertical())
        waypoints_ = getVerticalFlightPath(missionBounds);
    else
        waypoints_ = getHorizontalFlightPath(missionBounds);

    for (auto i = 0; i < waypoints_.size() - 1; i++)
    {
        auto p1 = waypoints_[i];
        auto p2 = waypoints_[i+1];
        distance += getDistance(p1, p2);
    }

    if (distance > maxDistanceMeters_) return false;
    else return true;
}

QList<QGeoCoordinate> FlightPather::path() { return waypoints_; };