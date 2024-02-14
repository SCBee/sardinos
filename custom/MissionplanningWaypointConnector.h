#pragma once

#include <QColor>

#include <LmCdl/I_SimpleWaypointConnector.h>

class MissionPlanningWaypointConnector : public LmCdl::I_SimpleWaypointConnector
{
    Q_OBJECT
public:
    MissionPlanningWaypointConnector();

    QGeoCoordinate startLocation() const override;
    void setStartLocation(const QGeoCoordinate& newLocation);

    QGeoCoordinate endLocation() const override;
    void setEndLocation(const QGeoCoordinate& newLocation);

    QColor color() const override;
    void setColor(QColor color);

    bool visible() const override;
    void setVisible(bool isVisible);

    bool directionalIndicatorVisible() const override;
    void setDirectionalIndicatorVisible(bool isVisible);

    int relativeZOrder() const override;
    void setRelativeZOrder(int order);

private:
    Q_DISABLE_COPY(MissionPlanningWaypointConnector)

    QGeoCoordinate startLocation_;
    QGeoCoordinate endLocation_;

    QColor color_;
    bool visible_;
    bool directionalIndicatorVisible_;

    int relativeZOrder_;
};
