#include <MissionPlanningWaypointConnector.h>

MissionPlanningWaypointConnector::MissionPlanningWaypointConnector()
    : color_(Qt::red)
    , visible_(false)
    , directionalIndicatorVisible_(false)
    , relativeZOrder_(0)
{
}

QGeoCoordinate MissionPlanningWaypointConnector::startLocation() const
{
    return startLocation_;
}

void MissionPlanningWaypointConnector::setStartLocation(
    const QGeoCoordinate& location)
{
    if (startLocation_ != location) {
        startLocation_ = location;
        emit startLocationChanged(startLocation_);
    }
}

QGeoCoordinate MissionPlanningWaypointConnector::endLocation() const
{
    return endLocation_;
}

void MissionPlanningWaypointConnector::setEndLocation(
    const QGeoCoordinate& location)
{
    if (endLocation_ != location) {
        endLocation_ = location;
        emit startLocationChanged(endLocation_);
    }
}

QColor MissionPlanningWaypointConnector::color() const
{
    return color_;
}

void MissionPlanningWaypointConnector::setColor(const QColor& color)
{
    if (color != color_) {
        color_ = color;
        emit colorChanged(color_);
    }
}

bool MissionPlanningWaypointConnector::visible() const
{
    return visible_;
}

void MissionPlanningWaypointConnector::setVisible(bool isVisible)
{
    if (isVisible != visible_) {
        visible_ = isVisible;
        emit visibilityChanged(visible_);
    }
}

bool MissionPlanningWaypointConnector::directionalIndicatorVisible() const
{
    return directionalIndicatorVisible_;
}

void MissionPlanningWaypointConnector::setDirectionalIndicatorVisible(
    bool isVisible)
{
    if (isVisible != directionalIndicatorVisible_) {
        directionalIndicatorVisible_ = isVisible;
        emit directionalIndicatorVisibilityChanged(
            directionalIndicatorVisible_);
    }
}

int MissionPlanningWaypointConnector::relativeZOrder() const
{
    return relativeZOrder_;
}
