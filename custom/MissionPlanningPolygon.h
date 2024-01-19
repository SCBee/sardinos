#pragma once

#include <QGeoPolygon>

#include <LmCdl/VectorDataPolygonDrawing.h>

class MissionPlanningPolygon : public LmCdl::I_VectorDataPolygonDrawing
{
public:
    MissionPlanningPolygon(QGeoPolygon polygon);
    virtual ~MissionPlanningPolygon();

    QGeoPolygon polygon() const override;

    QPen pen() const override;

    QBrush brush() const override;

private:
    QGeoPolygon polygon_;
};