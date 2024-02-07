#include <QTimer>
#include <iostream>

#include <Drone.h>

Drone::Drone() {
    setLocation(QGeoCoordinate(51, -114, 1250));
    setVisible(true);
}

Drone::~Drone() {}

QGeoCoordinate Drone::location() const
{
    return location_;
}

void Drone::setLocation(QGeoCoordinate location)
{
    location_ = location;
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

void Drone::selected() {}

void Drone::deselected() {}

bool Drone::selectionEnabled() const
{
    return false;
}