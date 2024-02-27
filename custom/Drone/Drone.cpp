#include <QTimer>
#include <utility>

#include "Drone.h"

Drone::Drone(LmCdl::I_VcsiMapExtensionApi& mapApi)
    : visible_(false)
    , color_(deselectedColor_)
    , droneWidget_(new DroneWidget())
    , battery_(0.0)
{
    widget_ = &mapApi.addGraphicsWidget(droneWidget_);
}

void Drone::updateValues(double latitude,
                         double longitude,
                         double altitude,
                         double heading,
                         double speed,
                         double yaw,
                         double battery)
{
    setLocation(QGeoCoordinate(latitude, longitude, altitude));

    setHeading(
        LmCdl::WrappedAnglePlusMinusPi(heading, LmCdl::AngleUnit::Degrees));

    setSpeed(LmCdl::Speed(speed, LmCdl::SpeedUnit::MetersPerSecond));

    setYaw(LmCdl::WrappedAnglePlusMinusPi(yaw, LmCdl::AngleUnit::Degrees));

    setBattery(battery);

    droneWidget_->updateValues(
        latitude, longitude, altitude, heading, speed, yaw, battery);
}

QGeoCoordinate Drone::location() const
{
    return location_;
}

void Drone::setLocation(const QGeoCoordinate& location)
{
    location_ = location;
    widget_->setLocation(location_);
    widget_->setOffsetInPixels(QPoint(20, 20));
    emit locationChanged(location_);
}

LmCdl::WrappedAnglePlusMinusPi Drone::heading() const
{
    return heading_;
}

void Drone::setHeading(const LmCdl::WrappedAnglePlusMinusPi& heading)
{
    heading_ = heading;
    emit headingChanged(heading_);
}

QColor Drone::color() const
{
    return color_;
}

void Drone::setColor(QColor color)
{
    color_ = std::move(color);
    emit colorChanged(color_);
}

bool Drone::visible() const
{
    return visible_;
}

void Drone::setVisible(bool visible)
{
    visible_ = visible;
    emit visibleChanged(visible_);
}

void Drone::selected()
{
    setColor(selectedColor_);
    widget_->setVisible(true);
}

void Drone::deselected()
{
    setColor(deselectedColor_);
    widget_->setVisible(false);
}

bool Drone::selectionEnabled() const
{
    return true;
}

void Drone::setSpeed(const LmCdl::Speed& speed)
{
    speed_ = speed;
}

void Drone::setYaw(const LmCdl::WrappedAnglePlusMinusPi& yaw)
{
    yaw_ = yaw;
}

void Drone::setBattery(const double& battery)
{
    battery_ = battery;
}