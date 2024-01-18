#pragma once

#include <LmCdl/I_GeospatialSimpleWaypoint.h>

class MissionPlanningWaypoint : public LmCdl::I_GeospatialSimpleWaypoint
{
Q_OBJECT
public:
    MissionPlanningWaypoint(QGeoCoordinate location, QString label);
    MissionPlanningWaypoint(const MissionPlanningWaypoint& waypoint);
    virtual ~MissionPlanningWaypoint();

    /**
     * Provides the location the waypoint should be displayed at.
     */
    QGeoCoordinate location() const override;

    /**
     * Provides the shape of the displayed waypoint.
     */
    Shape shape() const override;

    /**
     * Provides the displayed label for the waypoint.
     */
    QString label() const override;

    /**
     * Provides the offset of the label relative to the center of the waypoint.
     * By default there is no offset, the label will be centered in the
     * waypoint. Override this function to provide custom label offsets.
     */
    QVector2D labelOffset() const override;

    /**
     * Provides the color for the displayed waypoint.
     */
    QColor color() const override;

    /**
     * Provides whether or not you want the waypoint to be visible on the map.
     */
    bool visible() const override;

    /**
     * Provides the priority this waypoint has over other simple waypoints that
     * are displayed in the same location as this one. Ex. If waypoint A has a
     * relative z-order of 0, waypoint B has a relative z-order of 1 and both
     * are requested to be drawn in the same location, then waypoint B will be
     * displayed in front of waypoint A.
     */
    int relativeZOrder() const override;

    /**
     * Called when the visual representation of this waypoint on the map is
     * selected by a user.
     */
    void selected() override;

    /**
     * Called when this waypoint changes to be no longer selected.
     */
    void deselected() override;

    /**
     * Provides whether or not the waypoint is selectable by a user.
     */
    bool selectionEnabled() const override;

    /**
     * Called when the visual representation of this waypoint on the map is
     * highlighted by a user.
     */
    bool highlighted() const override;

    /**
     * Provides whether or not the waypoint may be dragged by the user.
     */
    bool draggingEnabled() const override;

    /**
     * Event handler called when a drag operation begins for this waypoint.
     */
    void dragStarted() override;

    /**
     * Event handler called with location updates as the waypoint is dragged
     * around the map. This handler will often be invoked many times in a single
     * drag operation.
     *
     * \param dragLocation The current drag location. This will always have the
     * same MSL altitude as the waypoint itself.
     */
    void dragging(const QGeoCoordinate& dragLocation) override;

    /**
     * Event handler called when dragging is completed with a valid location.
     * The marks the end of the drag operation.
     *
     * \param dragLocation The final drag location. This will always have the
     * same MSL altitude as the waypoint itself.
     */
    void dragConfirmed(const QGeoCoordinate& dragLocation) override;

    /**
     * Event handler called when dragging is cancelled. This marks the end of
     * the drag operation.
     */
    void dragCancelled() override;

private:
    QGeoCoordinate location_;
    QString label_;
};