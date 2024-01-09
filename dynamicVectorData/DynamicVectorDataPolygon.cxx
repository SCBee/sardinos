/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <DynamicVectorDataPolygon.h>

DynamicVectorDataPolygon::DynamicVectorDataPolygon(QList<QGeoCoordinate> path)
    : polygon_(path)
{
}

DynamicVectorDataPolygon::~DynamicVectorDataPolygon() { }

QGeoPolygon DynamicVectorDataPolygon::polygon() const { return polygon_; }

QPen DynamicVectorDataPolygon::pen() const
{
    QPen pen = QPen(Qt::SolidLine);
    pen.setWidth(10);
    pen.setColor(QColor(0, 0, 255, 50));
    return pen;
}

QBrush DynamicVectorDataPolygon::brush() const { return QBrush(Qt::SolidPattern); }
