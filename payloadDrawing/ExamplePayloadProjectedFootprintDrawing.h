/**
\file
Copyright (c) 2020 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <LmCdl/I_PayloadProjectedFootprint.h>
#include <LmCdl/WrappedAnglePlusMinusPi.h>
#include <QObject>

class ExamplePayloadProjectedFootprintDrawing : public LmCdl::I_PayloadProjectedFootprint {
    Q_OBJECT
public:
    explicit ExamplePayloadProjectedFootprintDrawing();
    ~ExamplePayloadProjectedFootprintDrawing() override;

    void setVisible(bool visible);
    bool visible() const override;

    void setCameraLocation(const QGeoCoordinate& cameraLocation);
    QGeoCoordinate cameraLocation() const override;

    void setCameraOrientation(const QQuaternion& cameraOrientation);
    QQuaternion cameraOrientation() const override;

    void setCameraHorizontalFieldOfView(const LmCdl::WrappedAnglePlusMinusPi& cameraHorizontalFieldOfView);
    LmCdl::WrappedAnglePlusMinusPi cameraHorizontalFieldOfView() const override;

    void setCameraVerticalFieldOfView(const LmCdl::WrappedAnglePlusMinusPi& cameraVerticalFieldOfView);
    LmCdl::WrappedAnglePlusMinusPi cameraVerticalFieldOfView() const override;

    void setColor(const QColor& color);
    QColor color() const override;

private:
    Q_DISABLE_COPY(ExamplePayloadProjectedFootprintDrawing);

private:
    bool visible_;
    QGeoCoordinate cameraLocation_;
    QQuaternion cameraOrientation_;
    LmCdl::WrappedAnglePlusMinusPi cameraHorizontalFieldOfView_;
    LmCdl::WrappedAnglePlusMinusPi cameraVerticalFieldOfView_;
    QColor color_;
};
