/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <DynamicVectorDataDrawing.h>
#include <DynamicVectorDataDrawing.moc>

#include <DynamicVectorDataPolygon.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleLocation.h>
#include <vcstk/core/QtObjectUtility.h>

DynamicVectorDataDrawing::DynamicVectorDataDrawing(LmCdl::I_Vehicle& vehicle)
    : vehicle_(vehicle)
{
    vcsQtConnect(&vehicle.reportedLocation(), SIGNAL(locationChanged()), this, SLOT(updatePolygonLocation()));
}

DynamicVectorDataDrawing::~DynamicVectorDataDrawing() { }

const QSet<LmCdl::I_VectorDataPointDrawing*>& DynamicVectorDataDrawing::pointDrawings(const QGeoRectangle& area) const
{
    return points_;
}

const QSet<LmCdl::I_VectorDataLineDrawing*>& DynamicVectorDataDrawing::lineDrawings(const QGeoRectangle& area) const
{
    return lines_;
}

const QSet<LmCdl::I_VectorDataPolygonDrawing*>& DynamicVectorDataDrawing::polygonDrawings(
    const QGeoRectangle& area) const
{
    return polygons_;
}

bool DynamicVectorDataDrawing::visible() const { return true; }

void DynamicVectorDataDrawing::updatePolygonLocation()
{
    QGeoCoordinate location = vehicle_.reportedLocation().location();
    qDeleteAll(polygons_);
    polygons_.clear();
    polygons_.insert(
        new DynamicVectorDataPolygon(makePolygonPathAroundStartingLocation(location.latitude(), location.longitude())));
    emit drawingChanged();
}

QList<QGeoCoordinate> DynamicVectorDataDrawing::makePolygonPathAroundStartingLocation(
    double latitudeInDegrees, double longitudeInDegrees)
{
    QList<QGeoCoordinate> path;
    const double latOffset = 0.01;
    const double lonOffset = 0.03;
    path << QGeoCoordinate(latitudeInDegrees + latOffset, longitudeInDegrees + lonOffset)
         << QGeoCoordinate(latitudeInDegrees + latOffset, longitudeInDegrees - lonOffset)
         << QGeoCoordinate(latitudeInDegrees - latOffset, longitudeInDegrees - lonOffset)
         << QGeoCoordinate(latitudeInDegrees - latOffset, longitudeInDegrees + lonOffset);
    return path;
}
