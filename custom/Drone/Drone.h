#pragma once

#include <QGeoCoordinate>
#include <QImage>
#include <QObject>
#include <QTimer>

#include <LmCdl/I_Billboard.h>
#include <LmCdl/I_GraphicsWidget.h>
#include <LmCdl/I_TrackVisualization.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/Speed.h>

#include "DroneWidget.h"

class Drone : public LmCdl::I_TrackVisualization
{
    Q_OBJECT
public:
    explicit Drone(LmCdl::I_VcsiMapExtensionApi& mapApi);

    ~Drone() override = default;

    void updateValues(double latitude,
                      double longitude,
                      double altitude,
                      double heading,
                      double speed,
                      double yaw,
                      double battery);

    [[nodiscard]] QGeoCoordinate location() const override;

    void setLocation(const QGeoCoordinate& location);

    [[nodiscard]] LmCdl::WrappedAnglePlusMinusPi heading() const override;

    void setHeading(const LmCdl::WrappedAnglePlusMinusPi& heading);

    [[nodiscard]] QColor color() const override;

    void setColor(QColor color);

    [[nodiscard]] bool visible() const override;

    void setVisible(bool visible);

    void selected() override;

    void deselected() override;

    [[nodiscard]] bool selectionEnabled() const override;

    void setSpeed(const LmCdl::Speed& speed);

    void setYaw(const LmCdl::WrappedAnglePlusMinusPi& yaw);

    void setBattery(const double& battery);

private:
    const QColor selectedColor_   = QColor(50, 150, 255);
    const QColor deselectedColor_ = QColor(0, 50, 120);

    QGeoCoordinate location_;
    LmCdl::WrappedAnglePlusMinusPi heading_;
    LmCdl::Speed speed_;
    LmCdl::WrappedAnglePlusMinusPi yaw_;
    [[maybe_unused]] double battery_;
    QColor color_;
    bool visible_;

    LmCdl::I_GraphicsWidget* widget_;
    DroneWidget* droneWidget_;
};