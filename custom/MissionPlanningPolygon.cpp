#include <MissionPlanningPolygon.h>

MissionPlanningPolygon::MissionPlanningPolygon(const QGeoPolygon& polygon, const QColor& color)
    : polygon_(polygon)
{
    this->color_ = color;
}

MissionPlanningPolygon::~MissionPlanningPolygon() = default;

QGeoPolygon MissionPlanningPolygon::polygon() const
{
    return polygon_;
}

QPen MissionPlanningPolygon::pen() const
{
    QPen pen = QPen(Qt::SolidLine);
    pen.setWidth(4);
    pen.setColor(this->color_);
    return pen;
}

QBrush MissionPlanningPolygon::brush() const
{
    return {Qt::NoBrush};
}
