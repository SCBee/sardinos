#pragma once

#include <QGeoPolygon>

#include <LmCdl/VectorDataPolygonDrawing.h>

class MissionPlanningPolygon : public LmCdl::I_VectorDataPolygonDrawing
{
public:
    explicit MissionPlanningPolygon(const QGeoPolygon& polygon);
    ~MissionPlanningPolygon() override;

    [[nodiscard]] QGeoPolygon polygon() const override;

    [[nodiscard]] QPen pen() const override;

    [[nodiscard]] QBrush brush() const override;

private:
    QGeoPolygon polygon_;
};