#pragma once

#include <QGeoCoordinate>
#include <QImage>
#include <QObject>

#include <LmCdl/I_Billboard.h>
#include <LmCdl/I_TrackVisualization.h>

class Drone : public LmCdl::I_TrackVisualization
{
    Q_OBJECT
public:
    explicit Drone();

    ~Drone();

    QGeoCoordinate location() const override;

    void setLocation(const QGeoCoordinate& location);

    LmCdl::WrappedAnglePlusMinusPi heading() const override;

    void setHeading(LmCdl::WrappedAnglePlusMinusPi heading);

    QColor color() const override;

    void setColor(QColor color);

    bool visible() const override;

    void setVisible(bool visible);

    void selected() override;

    void deselected() override;

    bool selectionEnabled() const override;

signals:

    void locationChanged(const QGeoCoordinate& location);

    void headingChanged(const LmCdl::WrappedAnglePlusMinusPi& heading);

    void colorChanged(const QColor& color);

    void visibleChanged(bool visible);

    void selectionEnabledChanged(bool selectionEnabled);

private:
    QGeoCoordinate location_;
    LmCdl::WrappedAnglePlusMinusPi heading_;
    QColor color_;
    bool visible_;
};