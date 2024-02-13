#pragma once

#include <QGeoCoordinate>
#include <QImage>
#include <QObject>
#include <QTimer>

#include <DroneWidget.h>
#include <LmCdl/I_Billboard.h>
#include <LmCdl/I_GraphicsWidget.h>
#include <LmCdl/I_TrackVisualization.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/Speed.h>

class Drone : public LmCdl::I_TrackVisualization
{
    Q_OBJECT
public:
    explicit Drone(LmCdl::I_VcsiMapExtensionApi& mapApi);

    ~Drone();

    void updateValues(double latitude,
                      double longitude,
                      double altitude,
                      double heading,
                      double speed,
                      double yaw,
                      double battery);

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

    void setSpeed(LmCdl::Speed speed);

    void setYaw(LmCdl::WrappedAnglePlusMinusPi yaw);

    void setBattery(double battery);

signals:

    void locationChanged(const QGeoCoordinate& location);

    void headingChanged(const LmCdl::WrappedAnglePlusMinusPi& heading);

    void colorChanged(const QColor& color);

    void visibleChanged(bool visible);

    void selectionEnabledChanged(bool selectionEnabled);

private:
    const QColor selectedColor_ = QColor(50, 150, 255);
    const QColor deselectedColor_ = QColor(0, 50, 120);

    QGeoCoordinate location_;
    LmCdl::WrappedAnglePlusMinusPi heading_;
    LmCdl::Speed speed_;
    LmCdl::WrappedAnglePlusMinusPi yaw_;
    double battery_;
    QColor color_;
    bool visible_;

    LmCdl::I_GraphicsWidget* widget_;
    DroneWidget* droneWidget_;
};