#pragma once

#include <QGeoCoordinate>
#include <QGeoPolygon>
#include <QList>

#include <LmCdl/VectorDataLineDrawing.h>

class Line : public LmCdl::I_VectorDataLineDrawing
{
public:
    Line(const QGeoCoordinate& start, const QGeoCoordinate& end);
    ~Line() override;

    [[nodiscard]] QGeoPath path() const override;

    [[nodiscard]] QPen pen() const override;

    [[maybe_unused]] void updateStart(const QGeoCoordinate& newStart);
    [[maybe_unused]] void updateEnd(const QGeoCoordinate& newEnd);

private:
    QGeoCoordinate start_;
    QGeoCoordinate end_;
    QList<QGeoCoordinate> path_ = *new QList<QGeoCoordinate>();
};