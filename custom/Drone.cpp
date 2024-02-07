#include <QTimer>

#include <Drone.h>

Drone::Drone(LmCdl::I_Billboard& billboard, QGeoCoordinate* updatingLocation)
    : billboard_(&billboard)
    , locationToGoTo_(updatingLocation)
{
    billboard_->setVisible(true);
    
    timer_ = new QTimer();

    timer_->setInterval(1000);

    connect(timer_, &QTimer::timeout, this, &Drone::setLocation);
};

void Drone::setLocation()
{
    billboard_->setLocation(*locationToGoTo_);
}