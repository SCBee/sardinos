#pragma once

#include <QGeoCoordinate>
#include <vector>
#include <BoundingBox.h>

#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/I_GroundElevationApi.h>


class MathExt
{
public:
    void cvhull(std::vector<std::vector<QGeoCoordinate>>& pois);
    BoundingBox findSmallestBoundingBox(const QList<LmCdl::VcsiIdentifiedPointOfInterest>& points, LmCdl::I_GroundElevationApi& elevationApi);
    void delay(int ms);
    double getDistance(double lat1, double lon1, double lat2, double lon2);
    double getDistance(QGeoCoordinate c1, QGeoCoordinate c2);
    bool isVertical(BoundingBox box);

private:
    std::vector<double> sqPolar(QGeoCoordinate& point, QGeoCoordinate& com);
};