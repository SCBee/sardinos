/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_VectorDataPolygonDrawing.h>

#include <QGeoCoordinate>
#include <QList>
#include <QtGlobal>

class DynamicVectorDataPolygon : public LmCdl::I_VectorDataPolygonDrawing {
public:
    DynamicVectorDataPolygon(QList<QGeoCoordinate> path);
    virtual ~DynamicVectorDataPolygon();

    QGeoPolygon polygon() const override;
    QPen pen() const override;
    QBrush brush() const override;

private:
    Q_DISABLE_COPY(DynamicVectorDataPolygon)

    QGeoPolygon polygon_;
};
