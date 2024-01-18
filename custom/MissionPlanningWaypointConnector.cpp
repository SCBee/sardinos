#include <MissionplanningWaypointConnector.h>
#include <qcolor.h>

MissionPlanningWaypointConnector::MissionPlanningWaypointConnector(
    QGeoCoordinate start, QGeoCoordinate end)
    : start_(start)
    , end_(end)
{
}

MissionPlanningWaypointConnector::~MissionPlanningWaypointConnector() {}

QGeoCoordinate MissionPlanningWaypointConnector::startLocation() const
{
    return start_;
}

QGeoCoordinate MissionPlanningWaypointConnector::endLocation() const
{
    return end_;
}

QColor MissionPlanningWaypointConnector::color() const
{
    return QColor(255, 255, 255, 180);
}

bool MissionPlanningWaypointConnector::visible() const
{
    return true;
}

bool MissionPlanningWaypointConnector::directionalIndicatorVisible() const
{
    return true;
}

int MissionPlanningWaypointConnector::relativeZOrder() const
{
    return 0;
}