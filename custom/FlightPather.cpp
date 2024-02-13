#include <FlightPather.h>
#include <MathExt.h>
#include <qmath.h>

QList<QGeoCoordinate> sardinos::FlightPather::getVerticalFlightPath(
    BoundingBox missionBounds)
{
    auto longSpreadMeters =
        sardinos::MathExt::getDistance(missionBounds.NE, missionBounds.NW);

    auto longSpread =
        abs(missionBounds.eastBound() - missionBounds.westBound());

    auto steps = std::max(1.0, longSpreadMeters / SCANWIDTHMETERS);

    auto stepLongitude = longSpread / steps;

    auto currentLocation = missionBounds.SW;

    currentLocation.setLongitude(currentLocation.longitude()
                                 + (stepLongitude / 2.0));

    auto wayPoints = QList<QGeoCoordinate>();

    auto goingUp = true;

    while (currentLocation.longitude() < missionBounds.eastBound()) {
        wayPoints.append(currentLocation);

        if (goingUp && currentLocation.latitude() == missionBounds.southBound())
        {
            currentLocation.setLatitude(missionBounds.northBound());
            continue;
        }

        if (goingUp && currentLocation.latitude() == missionBounds.northBound())
        {
            currentLocation.setLongitude(currentLocation.longitude()
                                         + stepLongitude);
            goingUp = false;
            continue;
        }

        if (!goingUp
            && currentLocation.latitude() == missionBounds.northBound())
        {
            currentLocation.setLatitude(missionBounds.southBound());
            continue;
        }

        if (!goingUp
            && currentLocation.latitude() == missionBounds.southBound())
        {
            currentLocation.setLongitude(currentLocation.longitude()
                                         + stepLongitude);
            goingUp = true;
            continue;
        }
    }

    return wayPoints;
}

QList<QGeoCoordinate> sardinos::FlightPather::getHorizontalFlightPath(
    BoundingBox missionBounds)
{
    auto latSpreadMeters =
        sardinos::MathExt::getDistance(missionBounds.NE, missionBounds.SE);

    auto latSpread =
        abs(missionBounds.northBound() - missionBounds.southBound());

    auto steps = std::max(1.0, latSpreadMeters / SCANWIDTHMETERS);

    auto stepLatitude = latSpread / steps;

    auto currentLocation = missionBounds.SW;

    currentLocation.setLatitude(currentLocation.latitude()
                                + (stepLatitude / 2.0));

    auto wayPoints = QList<QGeoCoordinate>();

    auto goingRight = true;

    while (currentLocation.latitude() < missionBounds.northBound()) {
        wayPoints.append(currentLocation);

        if (goingRight
            && currentLocation.longitude() == missionBounds.westBound())
        {
            currentLocation.setLongitude(missionBounds.eastBound());
            continue;
        }

        if (goingRight
            && currentLocation.longitude() == missionBounds.eastBound())
        {
            currentLocation.setLatitude(currentLocation.latitude()
                                        + stepLatitude);
            goingRight = false;
            continue;
        }

        if (!goingRight
            && currentLocation.longitude() == missionBounds.eastBound())
        {
            currentLocation.setLongitude(missionBounds.westBound());
            continue;
        }

        if (!goingRight
            && currentLocation.longitude() == missionBounds.westBound())
        {
            currentLocation.setLatitude(currentLocation.latitude()
                                        + stepLatitude);
            goingRight = true;
            continue;
        }
    }

    return wayPoints;
}

QList<QGeoCoordinate> sardinos::FlightPather::getPath(BoundingBox missionBounds)
{
    if (sardinos::MathExt::isVertical(missionBounds))
        return getVerticalFlightPath(missionBounds);
    else
        return getHorizontalFlightPath(missionBounds);
}

bool sardinos::FlightPather::canFly(QList<MissionPlanningWaypoint*> waypoints)
{
    auto sum = 0.0;

    for (auto i = 0; i < waypoints.size() - 1; i++) {
        sum += sardinos::MathExt::getDistance(waypoints[i]->location(),
                                              waypoints[i + 1]->location());
    }

    return sum <= MAXDISTANCEMETERS;
}
