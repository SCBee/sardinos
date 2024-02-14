#include <MissionPlanningPolygon.h>

MissionPlanningPolygon::MissionPlanningPolygon(const QGeoPolygon& polygon)
    : polygon_(polygon)
{
}

MissionPlanningPolygon::~MissionPlanningPolygon() = default;

QGeoPolygon MissionPlanningPolygon::polygon() const
{
    return polygon_;
}

QPen MissionPlanningPolygon::pen() const
{
    QPen pen = QPen(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(QColor(255, 255, 255, 200));
    return pen;
}

QBrush MissionPlanningPolygon::brush() const
{
    return {Qt::NoBrush};
}
