#pragma once

#include <LmCdl/I_SimpleWaypointConnector.h>

class MissionPlanningWaypointConnector : public LmCdl::I_SimpleWaypointConnector
{
public:
    MissionPlanningWaypointConnector(QGeoCoordinate start, QGeoCoordinate end);
    MissionPlanningWaypointConnector(const MissionPlanningWaypointConnector& connector);
    virtual ~MissionPlanningWaypointConnector();
    /**
     * Provides the location the connector should start at.
     */
    QGeoCoordinate startLocation() const override;

    /**
     * Provides the location the connector should end at.
     */
    QGeoCoordinate endLocation() const override;

    /**
     * Provides the color for the displayed connector.
     */
    QColor color() const override;

    /**
     * Provides whether or not you want the connector to be visible on the map.
     */
    bool visible() const override;

    /**
     *  Provides whether or not you want a directional arrow to be visible when
     * the connector is visible
     */
    bool directionalIndicatorVisible() const override;

    /**
     * Provides the priority this connector has over other simple connectors
     * that are displayed in the same location as this one. Ex. If connector A
     * has a relative z-order of 0, connector B has a relative z-order of 1 and
     * both are requested to be drawn in the same location, then connector B
     * will be displayed in front of connector A. The drawing will only respect
     * z orders in the range of [0-5], with any value below this range being
     * bounded to the minimum and any values above this range being bounded to
     * the maximum.
     */
    int relativeZOrder() const override;

private:
    QGeoCoordinate start_;
    QGeoCoordinate end_;
};