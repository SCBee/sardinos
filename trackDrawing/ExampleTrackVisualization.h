/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_TrackVisualization.h>

#include <LmCdl/WrappedAnglePlusMinusPi.h>
#include <QColor>
#include <QGeoCoordinate>

class ExampleTrackVisualization : public LmCdl::I_TrackVisualization {
    Q_OBJECT
public:
    explicit ExampleTrackVisualization();
    ~ExampleTrackVisualization();

    void setLocation(const QGeoCoordinate& location);
    QGeoCoordinate location() const override;

    void setHeading(const LmCdl::WrappedAnglePlusMinusPi& heading);
    LmCdl::WrappedAnglePlusMinusPi heading() const override;

    void setColor(const QColor& color);
    QColor color() const override;

    void setVisible(bool visible);
    bool visible() const override;

    void selected() override;
    void deselected() override;

    void setSelectionEnabled(bool selectionEnabled);
    bool selectionEnabled() const override;

private:
    Q_DISABLE_COPY(ExampleTrackVisualization)

    QGeoCoordinate location_;
    LmCdl::WrappedAnglePlusMinusPi heading_;
    QColor color_;
    bool visible_;
    bool selectionEnabled_;
};
