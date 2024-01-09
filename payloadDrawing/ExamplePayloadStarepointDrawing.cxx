/**
 \file
 Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
 */

#include <ExamplePayloadStarepointDrawing.h>

ExamplePayloadStarepointDrawing::ExamplePayloadStarepointDrawing()
    : visible_(false)
    , color_(Qt::white)
    , relativeZOrder_(0)
{
}

void ExamplePayloadStarepointDrawing::setVisible(const bool visible)
{
    if (visible != visible_) {
        visible_ = visible;
        emit visibilityChanged(visible);
    }
}

bool ExamplePayloadStarepointDrawing::visible() const { return visible_; }

void ExamplePayloadStarepointDrawing::setLocation(const QGeoCoordinate& location)
{
    if (location != location_) {
        location_ = location;
        emit locationChanged(location);
    }
}

QGeoCoordinate ExamplePayloadStarepointDrawing::location() const { return location_; }

void ExamplePayloadStarepointDrawing::setColor(const QColor& color)
{
    if (color != color_) {
        color_ = color;
        emit colorChanged(color);
    }
}

QColor ExamplePayloadStarepointDrawing::color() const { return color_; }

void ExamplePayloadStarepointDrawing::setRelativeZOrder(const int zOrder)
{
    if (zOrder != relativeZOrder_) {
        relativeZOrder_ = zOrder;
        emit relativeZOrderChanged(zOrder);
    }
}

int ExamplePayloadStarepointDrawing::relativeZOrder() const { return relativeZOrder_; }
