/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_VectorDataDrawing.h>

#include <QtGlobal>

class QGeoCoordinate;
namespace LmCdl {
class I_Vehicle;
}

class DynamicVectorDataDrawing : public LmCdl::I_VectorDataDrawing {
    Q_OBJECT
public:
    explicit DynamicVectorDataDrawing(LmCdl::I_Vehicle& vehicle);
    virtual ~DynamicVectorDataDrawing();

    const QSet<LmCdl::I_VectorDataPointDrawing*>& pointDrawings(const QGeoRectangle& area) const override;
    const QSet<LmCdl::I_VectorDataLineDrawing*>& lineDrawings(const QGeoRectangle& area) const override;
    const QSet<LmCdl::I_VectorDataPolygonDrawing*>& polygonDrawings(const QGeoRectangle& area) const override;
    bool visible() const override;

private slots:
    void updatePolygonLocation();

private:
    QList<QGeoCoordinate> makePolygonPathAroundStartingLocation(double latitudeInDegrees, double longitudeInDegrees);

private:
    Q_DISABLE_COPY(DynamicVectorDataDrawing)

    LmCdl::I_Vehicle& vehicle_;

    QSet<LmCdl::I_VectorDataPointDrawing*> points_;
    QSet<LmCdl::I_VectorDataLineDrawing*> lines_;
    QSet<LmCdl::I_VectorDataPolygonDrawing*> polygons_;
};
