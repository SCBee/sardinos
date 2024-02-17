#pragma once

#include <QColor>

#include <LmCdl/I_SimpleWaypointConnector.h>

class MissionPlanningWaypointConnector : public LmCdl::I_SimpleWaypointConnector
{
    Q_OBJECT
public:
    MissionPlanningWaypointConnector();

    [[nodiscard]] QGeoCoordinate startLocation() const override;
    void setStartLocation(const QGeoCoordinate& newLocation);

    [[nodiscard]] QGeoCoordinate endLocation() const override;
    void setEndLocation(const QGeoCoordinate& newLocation);

    [[nodiscard]] QColor color() const override;
    void setColor(const QColor& color);

    [[nodiscard]] bool visible() const override;
    void setVisible(bool isVisible);

    [[nodiscard]] bool directionalIndicatorVisible() const override;
    void setDirectionalIndicatorVisible(bool isVisible);

    [[nodiscard]] int relativeZOrder() const override;

private:
    Q_DISABLE_COPY(MissionPlanningWaypointConnector)

    QGeoCoordinate startLocation_;
    QGeoCoordinate endLocation_;

    QColor color_;
    bool visible_;
    bool directionalIndicatorVisible_;

    int relativeZOrder_;
};
