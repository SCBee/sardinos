#include <LmCdl/I_SimpleWaypointConnector.h>
#include <MissionplanningWaypointConnector.h>
#include <qcolor.h>

MissionPlanningWaypointConnector::MissionPlanningWaypointConnector(
    QGeoCoordinate start, QGeoCoordinate end)
    : start_(start)
    , end_(end)
{
}

MissionPlanningWaypointConnector::MissionPlanningWaypointConnector(
    const MissionPlanningWaypointConnector& connector)
    : start_(connector.startLocation())
    , end_(connector.endLocation())
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