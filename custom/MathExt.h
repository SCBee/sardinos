#pragma once

#include <QGeoCoordinate>
#include <vector>
#include <qmath.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <BoundingBox.h>

class MathExt
{
public:
    void cvhull(std::vector<std::vector<QGeoCoordinate>> &pois);
    BoundingBox findSmallestBoundingBox(const QList<LmCdl::VcsiIdentifiedPointOfInterest> &points);
    void delay(int ms);

private:
    std::vector<double> sqPolar(QGeoCoordinate &point, QGeoCoordinate &com);
};