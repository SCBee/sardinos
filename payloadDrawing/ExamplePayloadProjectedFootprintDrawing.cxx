/**
\file
Copyright (c) 2020 Lockheed Martin Corp.  All rights reserved.
*/

#include <ExamplePayloadProjectedFootprintDrawing.h>

ExamplePayloadProjectedFootprintDrawing::ExamplePayloadProjectedFootprintDrawing()
    : visible_(true)
{
}

ExamplePayloadProjectedFootprintDrawing::~ExamplePayloadProjectedFootprintDrawing() { }

void ExamplePayloadProjectedFootprintDrawing::setVisible(bool visible)
{
    if (visible_ != visible) {
        visible_ = visible;
        emit visibilityChanged();
    }
}

bool ExamplePayloadProjectedFootprintDrawing::visible() const { return visible_; }

void ExamplePayloadProjectedFootprintDrawing::setCameraLocation(const QGeoCoordinate& cameraLocation)
{
    if (cameraLocation_ != cameraLocation) {
        cameraLocation_ = cameraLocation;
        emit cameraOrientationChanged();
    }
}

QGeoCoordinate ExamplePayloadProjectedFootprintDrawing::cameraLocation() const { return cameraLocation_; }

void ExamplePayloadProjectedFootprintDrawing::setCameraOrientation(const QQuaternion& cameraOrientation)
{
    if (cameraOrientation_ != cameraOrientation) {
        cameraOrientation_ = cameraOrientation;
        emit cameraOrientationChanged();
    }
}

QQuaternion ExamplePayloadProjectedFootprintDrawing::cameraOrientation() const { return cameraOrientation_; }

void ExamplePayloadProjectedFootprintDrawing::setCameraHorizontalFieldOfView(
    const LmCdl::WrappedAnglePlusMinusPi& cameraHorizontalFieldOfView)
{
    if (cameraHorizontalFieldOfView_ != cameraHorizontalFieldOfView) {
        cameraHorizontalFieldOfView_ = cameraHorizontalFieldOfView;
        emit cameraHorizontalFieldOfViewChanged();
    }
}

LmCdl::WrappedAnglePlusMinusPi ExamplePayloadProjectedFootprintDrawing::cameraHorizontalFieldOfView() const
{
    return cameraHorizontalFieldOfView_;
}

void ExamplePayloadProjectedFootprintDrawing::setCameraVerticalFieldOfView(
    const LmCdl::WrappedAnglePlusMinusPi& cameraVerticalFieldOfView)
{
    if (cameraVerticalFieldOfView_ != cameraVerticalFieldOfView) {
        cameraVerticalFieldOfView_ = cameraVerticalFieldOfView;
        emit cameraVerticalFieldOfViewChanged();
    }
}

LmCdl::WrappedAnglePlusMinusPi ExamplePayloadProjectedFootprintDrawing::cameraVerticalFieldOfView() const
{
    return cameraVerticalFieldOfView_;
}

void ExamplePayloadProjectedFootprintDrawing::setColor(const QColor& color)
{
    if (color_ != color) {
        color_ = color;
        emit colorChanged();
    }
}

QColor ExamplePayloadProjectedFootprintDrawing::color() const { return color_; }
