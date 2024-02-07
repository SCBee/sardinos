#include <QTimer>

#include <Drone.h>
#include <iostream>


void Drone::init(LmCdl::I_Billboard& billboard){
    billboard_ = &billboard;

    billboard_->setVisible(false);
}

void Drone::show(){
    billboard_->setVisible(true);
}

void Drone::hide(){
    billboard_->setVisible(false);
}

void Drone::setLocation(double latitude, double longitude, double altitude)
{
    billboard_->setLocation(QGeoCoordinate(latitude, longitude, altitude));

    std::cout << "Going to new location: " << latitude << ", " << longitude; 
}