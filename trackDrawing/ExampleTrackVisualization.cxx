/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <ExampleTrackVisualization.h>

ExampleTrackVisualization::ExampleTrackVisualization()
    : location_()
    , heading_()
    , color_()
    , visible_(true)
    , selectionEnabled_(true)
{
}

ExampleTrackVisualization::~ExampleTrackVisualization() { }

void ExampleTrackVisualization::setLocation(const QGeoCoordinate& location)
{
    if (location_ != location) {
        location_ = location;
        emit locationChanged(location_);
    }
}

QGeoCoordinate ExampleTrackVisualization::location() const { return location_; }

void ExampleTrackVisualization::setHeading(const LmCdl::WrappedAnglePlusMinusPi& heading)
{
    if (heading_ != heading) {
        heading_ = heading;
        emit headingChanged(heading_);
    }
}

LmCdl::WrappedAnglePlusMinusPi ExampleTrackVisualization::heading() const { return heading_; }

void ExampleTrackVisualization::setColor(const QColor& color)
{
    if (color_ != color) {
        color_ = color;
        emit colorChanged(color_);
    }
}

QColor ExampleTrackVisualization::color() const { return color_; }

void ExampleTrackVisualization::setVisible(bool visible)
{
    if (visible_ != visible) {
        visible_ = visible;
        emit visibleChanged(visible_);
    }
}

bool ExampleTrackVisualization::visible() const { return visible_; }

void ExampleTrackVisualization::selected() { }

void ExampleTrackVisualization::deselected() { }

void ExampleTrackVisualization::setSelectionEnabled(bool selectionEnabled)
{
    if (selectionEnabled_ != selectionEnabled) {
        selectionEnabled_ = selectionEnabled;
        emit selectionEnabledChanged(selectionEnabled_);
    }
}

bool ExampleTrackVisualization::selectionEnabled() const { return selectionEnabled_; }