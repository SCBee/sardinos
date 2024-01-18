#include <QColor>

#include <MissionPlanningWaypoint.h>

MissionPlanningWaypoint::MissionPlanningWaypoint(QGeoCoordinate location,
                                                 QString label)
    : location_(location)
    , label_(label)
{
}

MissionPlanningWaypoint::~MissionPlanningWaypoint() {}

QGeoCoordinate MissionPlanningWaypoint::location() const
{
    return location_;
}

LmCdl::I_GeospatialSimpleWaypoint::Shape MissionPlanningWaypoint::shape() const
{
    return Shape::Circle;
}

QString MissionPlanningWaypoint::label() const
{
    return label_;
}

QColor MissionPlanningWaypoint::color() const
{
    return QColor(255, 255, 255, 180);
}

bool MissionPlanningWaypoint::visible() const
{
    return true;
}

int MissionPlanningWaypoint::relativeZOrder() const
{
    return 0;
}

bool MissionPlanningWaypoint::draggingEnabled() const
{
    return true;
}

bool MissionPlanningWaypoint::selectionEnabled() const
{
    return false;
}

QVector2D MissionPlanningWaypoint::labelOffset() const
{
    return QVector2D();
}
