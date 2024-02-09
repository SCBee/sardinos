#include <QTimer>
#include <iostream>

#include <Drone.h>
#include <DroneWidget.h>

Drone::Drone(volatile double& latitude,
             volatile double& longitude,
             volatile double& altitude,
             volatile double& heading,
             volatile double& speed,
             volatile double& yaw,
             volatile double& battery,
             LmCdl::I_VcsiMapExtensionApi& mapApi)
    : visible_(true)
    , color_(deselectedColor_)
{
    auto droneWidget = new DroneWidget();

    widget_ = &mapApi.addGraphicsWidget(droneWidget);

    timer = new QTimer();
    timer->setInterval(1000);

    connect(
        timer,
        &QTimer::timeout,
        this,
        [=]()
        {
            setLocation(QGeoCoordinate(std::ref(latitude),
                                       std::ref(longitude),
                                       std::ref(altitude) + 1219));

            setHeading(LmCdl::WrappedAnglePlusMinusPi(
                std::ref(heading), LmCdl::AngleUnit::Degrees));

            setSpeed(LmCdl::Speed(speed, LmCdl::SpeedUnit::MetersPerSecond));

            setYaw(
                LmCdl::WrappedAnglePlusMinusPi(yaw, LmCdl::AngleUnit::Degrees));

            setBattery(battery);

            droneWidget->updateValues(latitude, longitude, altitude, heading, speed, yaw, battery);
        });

    timer->start();
}

Drone::~Drone() {}

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

void Drone::setHeading(LmCdl::WrappedAnglePlusMinusPi heading)
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
    color_ = color;
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

void Drone::setSpeed(LmCdl::Speed speed)
{
    speed_ = speed;
}

void Drone::setYaw(LmCdl::WrappedAnglePlusMinusPi yaw)
{
    yaw_ = yaw;
}

void Drone::setBattery(double battery)
{
    battery_ = battery;
}