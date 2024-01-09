#pragma once

#include <LmCdl/VectorDataPolygonDrawing.h>
#include <QGeoPolygon>

class MissionPlanningPolygon : public LmCdl::I_VectorDataPolygonDrawing {

public:
    MissionPlanningPolygon(QGeoPolygon polygon);
    virtual ~MissionPlanningPolygon();

    QGeoPolygon polygon() const override;

    QPen pen() const override;

    QBrush brush() const override;

private:
    QGeoPolygon polygon_;
};