#pragma once

#include <LmCdl/I_SimpleWaypointConnector.h>

class MissionPlanningWaypointConnector : public LmCdl::I_SimpleWaypointConnector
{
public:
    MissionPlanningWaypointConnector(QGeoCoordinate start, QGeoCoordinate end);
    MissionPlanningWaypointConnector(const MissionPlanningWaypointConnector& connector);
    virtual ~MissionPlanningWaypointConnector();

    QGeoCoordinate startLocation() const override;

    QGeoCoordinate endLocation() const override;

    QColor color() const override;

    bool visible() const override;

    bool directionalIndicatorVisible() const override;

    int relativeZOrder() const override;

private:
    QGeoCoordinate start_;
    QGeoCoordinate end_;
};