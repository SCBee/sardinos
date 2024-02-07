#pragma once

#include <QGeoCoordinate>
#include <QImage>
#include <QObject>

#include <LmCdl/I_Billboard.h>

class Drone : public QObject
{
    Q_OBJECT
public:
    Drone(LmCdl::I_Billboard* billboard, QGeoCoordinate* updatingLocation);

    ~Drone() {}

    void setLocation();

private:
    LmCdl::I_Billboard* billboard_;

    QGeoCoordinate* locationToGoTo_;

    QTimer* timer_;
};