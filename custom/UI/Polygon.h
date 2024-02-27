#pragma once

#include <QGeoPolygon>

#include <LmCdl/VectorDataPolygonDrawing.h>

class Polygon : public LmCdl::I_VectorDataPolygonDrawing
{
public:
    explicit Polygon(const QGeoPolygon& polygon);
    ~Polygon() override;

    [[nodiscard]] QGeoPolygon polygon() const override;

    [[nodiscard]] QPen pen() const override;

    [[nodiscard]] QBrush brush() const override;

private:
    QGeoPolygon polygon_;
};