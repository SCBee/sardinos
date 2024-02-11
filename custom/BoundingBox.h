#pragma once

#include <QGeoCoordinate>

#include <LmCdl/I_GroundElevationApi.h>
#include <LmCdl/GroundElevation.h>
#include <LmCdl/DistanceUnit.h>

struct BoundingBox
{
public:
    BoundingBox(LmCdl::I_GroundElevationApi& elevationApi) 
    : elevationApi_(elevationApi)
    {
    }

    void setCoordinates(QGeoCoordinate SW,
                        QGeoCoordinate NW,
                        QGeoCoordinate SE,
                        QGeoCoordinate NE)
    {
        this->SW = SW;
        this->NW = NW;
        this->SE = SE;
        this->NE = NE;

        this->SW.setAltitude(elevationApi_.lookupGroundElevationSynchronously(this->SW).altitudeMsl().value(LmCdl::DistanceUnit::Meters));
        this->NW.setAltitude(elevationApi_.lookupGroundElevationSynchronously(this->NW).altitudeMsl().value(LmCdl::DistanceUnit::Meters));
        this->SE.setAltitude(elevationApi_.lookupGroundElevationSynchronously(this->SE).altitudeMsl().value(LmCdl::DistanceUnit::Meters));
        this->NE.setAltitude(elevationApi_.lookupGroundElevationSynchronously(this->NE).altitudeMsl().value(LmCdl::DistanceUnit::Meters));
    }

        void setCoordinates(BoundingBox box)
    {
        setCoordinates(box.SW, box.NW, box.SE, box.NE);
    }

    QList<QGeoCoordinate> list()
    {
        return {SW, SE, NE, NW};
    }

    double eastBound() { return NE.longitude(); }

    double northBound() { return NE.latitude(); }

    double westBound() { return SW.longitude(); }

    double southBound() { return SW.latitude(); }

    QGeoCoordinate SW;
    QGeoCoordinate NW;
    QGeoCoordinate SE;
    QGeoCoordinate NE;

private:
    LmCdl::I_GroundElevationApi& elevationApi_;
};