#include <MissionPlanningPolygon.h>


MissionPlanningPolygon::MissionPlanningPolygon(QGeoPolygon polygon)
    : polygon_(polygon)
{
}

MissionPlanningPolygon::~MissionPlanningPolygon(){}

QGeoPolygon MissionPlanningPolygon::polygon() const { return polygon_; }

QPen MissionPlanningPolygon::pen() const
{
    QPen pen = QPen(Qt::SolidLine);
    pen.setWidth(10);
    pen.setColor(QColor(0, 0, 50, 250));
    return pen;
}

QBrush MissionPlanningPolygon::brush() const { return QBrush(Qt::SolidPattern); }