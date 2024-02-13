#pragma once

#include <QGeoCoordinate>
#include <vector>

#include <BoundingBox.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>

namespace sardinos::MathExt
{
    void cvhull(std::vector<std::vector<QGeoCoordinate>>& pois);
    BoundingBox findSmallestBoundingBox(
        const QList<LmCdl::VcsiIdentifiedPointOfInterest>& points);
    void delay(int ms);
    double getDistance(double lat1, double lon1, double lat2, double lon2);
    double getDistance(const QGeoCoordinate& c1, const QGeoCoordinate& c2);
    bool isVertical(const BoundingBox& box);
    std::vector<double> sqPolar(QGeoCoordinate& point, QGeoCoordinate& com);
}  // namespace sardinos::MathExt