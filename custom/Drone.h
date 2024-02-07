#pragma once

#include <QGeoCoordinate>
#include <QImage>
#include <QObject>

#include <LmCdl/I_Billboard.h>

class Drone : public QObject
{
    Q_OBJECT
public:
    Drone() {}

    ~Drone() {}

    void setLocation(double latitude, double longitude, double altitude);
    void Drone::init(LmCdl::I_Billboard& billboard);
    void Drone::show();
    void Drone::hide();

private:
    LmCdl::I_Billboard* billboard_;

    QTimer* timer_;
};