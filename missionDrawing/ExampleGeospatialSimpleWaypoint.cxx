/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <ExampleGeospatialSimpleWaypoint.h>

ExampleGeospatialSimpleWaypoint::ExampleGeospatialSimpleWaypoint()
    : location_()
    , shape_(LmCdl::I_GeospatialSimpleWaypoint::Shape::Circle)
    , label_()
    , labelOffset_(QVector2D(0, 0))
    , color_()
    , visible_(true)
    , relativeZOrder_(0)
    , highlighted_(true)

    , selectionEnabled_(true)
    , draggingEnabled_(false)
{
}

ExampleGeospatialSimpleWaypoint::~ExampleGeospatialSimpleWaypoint() { }

void ExampleGeospatialSimpleWaypoint::setLocation(const QGeoCoordinate& location)
{
    if (location_ != location) {
        location_ = location;
        emit locationChanged(location_);
    }
}

QGeoCoordinate ExampleGeospatialSimpleWaypoint::location() const { return location_; }

void ExampleGeospatialSimpleWaypoint::setShape(const LmCdl::I_GeospatialSimpleWaypoint::Shape& shape)
{
    if (shape_ != shape) {
        shape_ = shape;
        emit shapeChanged(shape_);
    }
}

LmCdl::I_GeospatialSimpleWaypoint::Shape ExampleGeospatialSimpleWaypoint::shape() const { return shape_; }

void ExampleGeospatialSimpleWaypoint::setLabel(const QString& label)
{
    if (label_ != label) {
        label_ = label;
        emit labelChanged(label_);
    }
}

QString ExampleGeospatialSimpleWaypoint::label() const { return label_; }

void ExampleGeospatialSimpleWaypoint::setLabelOffset(const QVector2D& labelOffset)
{
    if (labelOffset_ != labelOffset) {
        labelOffset_ = labelOffset;
        emit labelOffsetChanged(labelOffset_);
    }
}

QVector2D ExampleGeospatialSimpleWaypoint::labelOffset() const { return labelOffset_; }

void ExampleGeospatialSimpleWaypoint::setColor(const QColor& color)
{
    if (color_ != color) {
        color_ = color;
        emit colorChanged(color_);
    }
}

QColor ExampleGeospatialSimpleWaypoint::color() const { return color_; }

void ExampleGeospatialSimpleWaypoint::setVisible(bool visible)
{
    if (visible_ != visible) {
        visible_ = visible;
        emit visibilityChanged(visible_);
    }
}

bool ExampleGeospatialSimpleWaypoint::visible() const { return visible_; }

void ExampleGeospatialSimpleWaypoint::setRelativeZOrder(int relativeZOrder)
{
    if (relativeZOrder_ != relativeZOrder) {
        relativeZOrder_ = relativeZOrder;
        emit relativeZOrderChanged(relativeZOrder_);
    }
}

int ExampleGeospatialSimpleWaypoint::relativeZOrder() const { return relativeZOrder_; }

void ExampleGeospatialSimpleWaypoint::selected() { setHighlighted(true); }

void ExampleGeospatialSimpleWaypoint::deselected() { setHighlighted(false); }

void ExampleGeospatialSimpleWaypoint::setSelectionEnabled(bool selectionEnabled)
{
    if (selectionEnabled_ != selectionEnabled) {
        selectionEnabled_ = selectionEnabled;
        emit selectionEnabledChanged(selectionEnabled_);
    }
}

bool ExampleGeospatialSimpleWaypoint::selectionEnabled() const { return selectionEnabled_; }

void ExampleGeospatialSimpleWaypoint::setHighlighted(bool highlighted)
{
    if (highlighted_ != highlighted) {
        highlighted_ = highlighted;
        emit highlightChanged(highlighted_);
    }
}

bool ExampleGeospatialSimpleWaypoint::highlighted() const { return highlighted_; }

void ExampleGeospatialSimpleWaypoint::setDraggingEnabled(bool dragging)
{
    if (dragging != draggingEnabled_) {
        draggingEnabled_ = dragging;
        emit draggingEnabledChanged(draggingEnabled_);
    }
}

bool ExampleGeospatialSimpleWaypoint::draggingEnabled() const { return draggingEnabled_; }

void ExampleGeospatialSimpleWaypoint::dragStarted() { emit dragStartedFromDrawing(); }

void ExampleGeospatialSimpleWaypoint::dragging(const QGeoCoordinate& dragLocation)
{
    emit draggingOccuredFromDrawing(dragLocation);
}

void ExampleGeospatialSimpleWaypoint::dragConfirmed(const QGeoCoordinate& dragLocation)
{
    emit dragConfirmedFromDrawing(dragLocation);
}

void ExampleGeospatialSimpleWaypoint::dragCancelled() { emit dragCancelledFromDrawing(); }
