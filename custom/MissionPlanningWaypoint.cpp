#include <QColor>

#include <LmCdl/I_GeospatialSimpleWaypoint.h>
#include <MissionPlanningWaypoint.h>

MissionPlanningWaypoint::MissionPlanningWaypoint(QGeoCoordinate location,
                                                 QString label)
    : location_(location)
    , label_(label)
{
}

MissionPlanningWaypoint::MissionPlanningWaypoint(
    const MissionPlanningWaypoint& waypoint)
    : location_(waypoint.location())
    , label_(waypoint.label())
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
    return QColor(0, 0, 255, 255);
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

void MissionPlanningWaypoint::selected() {}

void MissionPlanningWaypoint::deselected() {}

void MissionPlanningWaypoint::dragStarted() {}

void MissionPlanningWaypoint::dragging(const QGeoCoordinate& dragLocation) {}

void MissionPlanningWaypoint::dragConfirmed(const QGeoCoordinate& dragLocation)
{
}

void MissionPlanningWaypoint::dragCancelled() {}

bool MissionPlanningWaypoint::highlighted() const
{
    return false;
}

QVector2D MissionPlanningWaypoint::labelOffset() const
{
    return QVector2D();
}

QVector2D MissionPlanningWaypoint::defaultLabelOffsetBelow()
{
    return QVector2D();
}

QSize MissionPlanningWaypoint::size()
{
    return QSize();
}
