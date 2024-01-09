/**
 \file
 Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
 */

#include <ExampleSimpleWaypointConnector.h>

ExampleSimpleWaypointConnector::ExampleSimpleWaypointConnector()
    : color_(Qt::white)
    , visible_(false)
    , directionalIndicatorVisible_(false)
    , relativeZOrder_(0)
{
}

QGeoCoordinate ExampleSimpleWaypointConnector::startLocation() const { return startLocation_; }

void ExampleSimpleWaypointConnector::setStartLocation(const QGeoCoordinate& location)
{
    if (startLocation_ != location) {
        startLocation_ = location;
        emit startLocationChanged(startLocation_);
    }
}

QGeoCoordinate ExampleSimpleWaypointConnector::endLocation() const { return endLocation_; }

void ExampleSimpleWaypointConnector::setEndLocation(const QGeoCoordinate& location)
{
    if (endLocation_ != location) {
        endLocation_ = location;
        emit startLocationChanged(endLocation_);
    }
}

QColor ExampleSimpleWaypointConnector::color() const { return color_; }

void ExampleSimpleWaypointConnector::setColor(QColor color)
{
    if (color != color_) {
        color_ = color;
        emit colorChanged(color_);
    }
}

bool ExampleSimpleWaypointConnector::visible() const { return visible_; }

void ExampleSimpleWaypointConnector::setVisible(bool isVisible)
{
    if (isVisible != visible_) {
        visible_ = isVisible;
        emit visibilityChanged(visible_);
    }
}

bool ExampleSimpleWaypointConnector::directionalIndicatorVisible() const { return directionalIndicatorVisible_; }

void ExampleSimpleWaypointConnector::setDirectionalIndicatorVisible(bool isVisible)
{
    if (isVisible != directionalIndicatorVisible_) {
        directionalIndicatorVisible_ = isVisible;
        emit directionalIndicatorVisibilityChanged(directionalIndicatorVisible_);
    }
}

int ExampleSimpleWaypointConnector::relativeZOrder() const { return relativeZOrder_; }

void ExampleSimpleWaypointConnector::setRelativeZOrder(int order)
{
    if (order != relativeZOrder_) {
        relativeZOrder_ = order;
        emit relativeZOrderChanged(relativeZOrder_);
    }
}
