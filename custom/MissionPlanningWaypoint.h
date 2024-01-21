#pragma once

#include <LmCdl/I_GeospatialSimpleWaypoint.h>

class MissionPlanningWaypoint : public LmCdl::I_GeospatialSimpleWaypoint
{
public:
    MissionPlanningWaypoint(QGeoCoordinate location, QString label);
    MissionPlanningWaypoint(const MissionPlanningWaypoint &waypoint);
    virtual ~MissionPlanningWaypoint();

    QGeoCoordinate location() const override;

    Shape shape() const override;

    QString label() const override;

    QVector2D labelOffset() const override;

    QColor color() const override;

    bool visible() const override;

    int relativeZOrder() const override;

    void selected() override;

    void deselected() override;

    bool selectionEnabled() const override;

    bool highlighted() const override;

    bool draggingEnabled() const override;

    void dragStarted() override;

    void dragging(const QGeoCoordinate& dragLocation) override;

    void dragConfirmed(const QGeoCoordinate& dragLocation) override;

    void dragCancelled() override;
    
    static QVector2D defaultLabelOffsetBelow();
    
    static QSize size();

private:
    QGeoCoordinate location_;
    QString label_;
};