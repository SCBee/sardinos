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
    pen.setWidth(1);
    pen.setColor(QColor(255, 255, 255, 255));
    return pen;
}

QBrush MissionPlanningPolygon::brush() const { return QBrush(Qt::NoBrush); }
