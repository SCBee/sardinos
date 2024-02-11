#pragma once

#include <QGeoCoordinate>
#include <vector>
#include <qmath.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <BoundingBox.h>

const auto EARTH_RADIUS = 6371000;

class MathExt
{
public:
    void cvhull(std::vector<std::vector<QGeoCoordinate>> &pois);
    BoundingBox findSmallestBoundingBox(const QList<LmCdl::VcsiIdentifiedPointOfInterest> &points);
    void delay(int ms);
    double calculateSeparation(double lat1, double lon1, double lat2, double lon2);

private:
    std::vector<double> sqPolar(QGeoCoordinate &point, QGeoCoordinate &com);
};