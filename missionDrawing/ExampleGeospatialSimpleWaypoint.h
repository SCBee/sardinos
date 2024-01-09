/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_GeospatialSimpleWaypoint.h>

#include <QColor>
#include <QGeoCoordinate>
#include <QString>

class ExampleGeospatialSimpleWaypoint : public LmCdl::I_GeospatialSimpleWaypoint {
    Q_OBJECT
public:
    explicit ExampleGeospatialSimpleWaypoint();
    ~ExampleGeospatialSimpleWaypoint();

    void setLocation(const QGeoCoordinate& location);
    QGeoCoordinate location() const override;

    void setShape(const LmCdl::I_GeospatialSimpleWaypoint::Shape& shape);
    LmCdl::I_GeospatialSimpleWaypoint::Shape shape() const override;

    void setLabel(const QString& label);
    QString label() const override;

    void setLabelOffset(const QVector2D& labelOffset);
    QVector2D labelOffset() const override;

    void setColor(const QColor& color);
    QColor color() const override;

    void setVisible(bool visible);
    bool visible() const override;

    void setRelativeZOrder(int relativeZOrder);
    int relativeZOrder() const override;

    void selected() override;
    void deselected() override;

    void setSelectionEnabled(bool selectionEnabled);
    bool selectionEnabled() const override;

    void setHighlighted(bool highlighted);
    bool highlighted() const override;

    void setDraggingEnabled(bool dragging);
    bool draggingEnabled() const override;

    void dragStarted() override;
    void dragging(const QGeoCoordinate& dragLocation) override;
    void dragConfirmed(const QGeoCoordinate& dragLocation) override;
    void dragCancelled() override;

signals:
    void dragStartedFromDrawing();
    void draggingOccuredFromDrawing(const QGeoCoordinate& dragLocation);
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
