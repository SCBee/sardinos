#include "WaypointConnector.h"

WaypointConnector::WaypointConnector()
    : color_(Qt::red)
    , visible_(false)
    , directionalIndicatorVisible_(false)
    , relativeZOrder_(0)
{
}

QGeoCoordinate WaypointConnector::startLocation() const
{
    return startLocation_;
}

void WaypointConnector::setStartLocation(const QGeoCoordinate& location)
{
    if (startLocation_ != location) {
        startLocation_ = location;
        emit startLocationChanged(startLocation_);
    }
}

QGeoCoordinate WaypointConnector::endLocation() const
{
    return endLocation_;
}

void WaypointConnector::setEndLocation(const QGeoCoordinate& location)
{
    if (endLocation_ != location) {
        endLocation_ = location;
        emit startLocationChanged(endLocation_);
    }
}

QColor WaypointConnector::color() const
{
    return color_;
}

void WaypointConnector::setColor(const QColor& color)
{
    if (color != color_) {
        color_ = color;
        emit colorChanged(color_);
    }
}

bool WaypointConnector::visible() const
{
    return visible_;
}

void WaypointConnector::setVisible(bool isVisible)
{
    if (isVisible != visible_) {
        visible_ = isVisible;
        emit visibilityChanged(visible_);
    }
}

bool WaypointConnector::directionalIndicatorVisible() const
{
    return directionalIndicatorVisible_;
}

void WaypointConnector::setDirectionalIndicatorVisible(bool isVisible)
{
    if (isVisible != directionalIndicatorVisible_) {
        directionalIndicatorVisible_ = isVisible;
        emit directionalIndicatorVisibilityChanged(
            directionalIndicatorVisible_);
    }
}

int WaypointConnector::relativeZOrder() const
{
    return relativeZOrder_;
}
