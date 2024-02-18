#pragma once

#include <QGeoCoordinate>

#include <BoundingBox.h>
#include <LmCdl/I_MissionDrawingApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <MissionDomain.h>
#include <MissionPlanningDrawing.h>
#include <MissionPlanningDrawing.h>
#include <MissionPlanningLine.h>
#include <MissionPlanningPolygon.h>
#include <MissionPlanningWaypoint.h>

const double SCANWIDTHMETERS = 500.0;
const double MAXDISTANCEMETERS = 200000000.0;
const double TURNINGRADIUSMETERS = 200.0;

class FlightPather
{
public:
    FlightPather();
    ~FlightPather();

    QList<QGeoCoordinate> getVerticalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getHorizontalFlightPath(BoundingBox missionBounds);
    QList<QGeoCoordinate> getPath(BoundingBox missionBounds);
    bool canFly(QList<MissionPlanningWaypoint*> waypoints);
};