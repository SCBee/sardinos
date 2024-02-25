#include <QColor>

#include <Waypoint.h>

Waypoint::Waypoint()
    : location_()
    , shape_(LmCdl::I_GeospatialSimpleWaypoint::Shape::Triangle)
    , label_()
    , labelOffset_(QVector2D(0, 0))
    , color_(Qt::red)
    , visible_(true)
    , relativeZOrder_(0)
    , highlighted_(true)
    , selectionEnabled_(true)
    , draggingEnabled_(false)
{
}

Waypoint::~Waypoint() = default;

void Waypoint::setLocation(const QGeoCoordinate& location)
{
    if (location_ != location) {
        location_ = location;
        emit locationChanged(location_);
    }
}

QGeoCoordinate Waypoint::location() const
{
    return location_;
}

void Waypoint::setShape(
    const LmCdl::I_GeospatialSimpleWaypoint::Shape& shape)
{
    if (shape_ != shape) {
        shape_ = shape;
        emit shapeChanged(shape_);
    }
}

LmCdl::I_GeospatialSimpleWaypoint::Shape Waypoint::shape() const
{
    return shape_;
}

void Waypoint::setLabel(const QString& label)
{
    if (label_ != label) {
        label_ = label;
        emit labelChanged(label_);
    }
}

QString Waypoint::label() const
{
    return label_;
}

QVector2D Waypoint::labelOffset() const
{
    return labelOffset_;
}

void Waypoint::setColor(const QColor& color)
{
    if (color_ != color) {
        color_ = color;
        emit colorChanged(color_);
    }
}

QColor Waypoint::color() const
{
    return color_;
}

void Waypoint::setVisible(bool visible)
{
    if (visible_ != visible) {
        visible_ = visible;
        emit visibilityChanged(visible_);
    }
}

bool Waypoint::visible() const
{
    return visible_;
}

int Waypoint::relativeZOrder() const
{
    return relativeZOrder_;
}

void Waypoint::selected()
{
    setHighlighted(true);
}

void Waypoint::deselected()
{
    setHighlighted(false);
}

void Waypoint::setSelectionEnabled(bool selectionEnabled)
{
    if (selectionEnabled_ != selectionEnabled) {
        selectionEnabled_ = selectionEnabled;
        emit selectionEnabledChanged(selectionEnabled_);
    }
}

bool Waypoint::selectionEnabled() const
{
    return selectionEnabled_;
}

void Waypoint::setHighlighted(bool highlighted)
{
    if (highlighted_ != highlighted) {
        highlighted_ = highlighted;
        emit highlightChanged(highlighted_);
    }
}

bool Waypoint::highlighted() const
{
    return highlighted_;
}

void Waypoint::setDraggingEnabled(bool dragging)
{
    if (dragging != draggingEnabled_) {
        draggingEnabled_ = dragging;
        emit draggingEnabledChanged(draggingEnabled_);
    }
}

bool Waypoint::draggingEnabled() const
{
    return draggingEnabled_;
}

void Waypoint::dragStarted()
{
    emit dragStartedFromDrawing();
}

void Waypoint::dragging(const QGeoCoordinate& dragLocation)
{
    emit draggingOccurredFromDrawing(dragLocation);
}

void Waypoint::dragConfirmed(const QGeoCoordinate& dragLocation)
{
    emit dragConfirmedFromDrawing(dragLocation);
}

void Waypoint::dragCancelled()
{
    emit dragCancelledFromDrawing();
}