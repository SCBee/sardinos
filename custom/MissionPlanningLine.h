#pragma once

#include <QGeoCoordinate>
#include <QGeoPolygon>
#include <QList>

#include <LmCdl/VectorDataLineDrawing.h>

class MissionPlanningLine : public LmCdl::I_VectorDataLineDrawing
{
public:
    MissionPlanningLine(QGeoCoordinate start, QGeoCoordinate end);
    virtual ~MissionPlanningLine();

    QGeoPath path() const override;

    QPen pen() const override;

    void updateStart(QGeoCoordinate newStart);
    void updateEnd(QGeoCoordinate newEnd);

private:
    QGeoCoordinate start_;
    QGeoCoordinate end_;
    QList<QGeoCoordinate> path_ = *new QList<QGeoCoordinate>();
};