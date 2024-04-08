#pragma once

#include <QColor>

#include <LmCdl/I_GeospatialSimpleWaypoint.h>

class Waypoint : public LmCdl::I_GeospatialSimpleWaypoint
{
    Q_OBJECT
public:
    explicit Waypoint();
    ~Waypoint() override;

    void setLocation(const QGeoCoordinate& location);
    [[nodiscard]] QGeoCoordinate location() const override;

    void setShape(const LmCdl::I_GeospatialSimpleWaypoint::Shape& shape);
    [[nodiscard]] LmCdl::I_GeospatialSimpleWaypoint::Shape shape()
        const override;

    void setLabel(const QString& label);
    [[nodiscard]] QString label() const override;

    [[nodiscard]] QVector2D labelOffset() const override;

    void setColor(const QColor& color);
    [[nodiscard]] QColor color() const override;

    void setVisible(bool visible);
    [[nodiscard]] bool visible() const override;

    [[nodiscard]] int relativeZOrder() const override;

    void selected() override;
    void deselected() override;

    void setSelectionEnabled(bool selectionEnabled);
    [[nodiscard]] bool selectionEnabled() const override;

    void setHighlighted(bool highlighted);
    [[nodiscard]] bool highlighted() const override;

    void setDraggingEnabled(bool dragging);
    [[nodiscard]] bool draggingEnabled() const override;

    void dragStarted() override;
    void dragging(const QGeoCoordinate& dragLocation) override;
    void dragConfirmed(const QGeoCoordinate& dragLocation) override;
    void dragCancelled() override;

signals:
    void dragStartedFromDrawing();
    void draggingOccurredFromDrawing(const QGeoCoordinate& dragLocation);
    void dragConfirmedFromDrawing(const QGeoCoordinate& dragLocation);
    void dragCancelledFromDrawing();

private:
    QGeoCoordinate location_;
    LmCdl::I_GeospatialSimpleWaypoint::Shape shape_;
    QString label_;
    QVector2D labelOffset_;
    QColor color_;
    bool visible_;
    int relativeZOrder_;
    bool highlighted_;

    bool selectionEnabled_;
    bool draggingEnabled_;
};
