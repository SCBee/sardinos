#include <QColor>

#include <MissionPlanningWaypoint.h>

MissionPlanningWaypoint::MissionPlanningWaypoint()
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

MissionPlanningWaypoint::~MissionPlanningWaypoint() = default;

void MissionPlanningWaypoint::setLocation(const QGeoCoordinate& location)
{
    if (location_ != location) {
        location_ = location;
        emit locationChanged(location_);
    }
}

QGeoCoordinate MissionPlanningWaypoint::location() const
{
    return location_;
}

void MissionPlanningWaypoint::setShape(
    const LmCdl::I_GeospatialSimpleWaypoint::Shape& shape)
{
    if (shape_ != shape) {
        shape_ = shape;
        emit shapeChanged(shape_);
    }
}

LmCdl::I_GeospatialSimpleWaypoint::Shape MissionPlanningWaypoint::shape() const
{
    return shape_;
}

void MissionPlanningWaypoint::setLabel(const QString& label)
{
    if (label_ != label) {
        label_ = label;
        emit labelChanged(label_);
    }
}

QString MissionPlanningWaypoint::label() const
{
    return label_;
}

QVector2D MissionPlanningWaypoint::labelOffset() const
{
    return labelOffset_;
}

void MissionPlanningWaypoint::setColor(const QColor& color)
{
    if (color_ != color) {
        color_ = color;
        emit colorChanged(color_);
    }
}

QColor MissionPlanningWaypoint::color() const
{
    return color_;
}

void MissionPlanningWaypoint::setVisible(bool visible)
{
    if (visible_ != visible) {
        visible_ = visible;
        emit visibilityChanged(visible_);
    }
}

bool MissionPlanningWaypoint::visible() const
{
    return visible_;
}

int MissionPlanningWaypoint::relativeZOrder() const
{
    return relativeZOrder_;
}

void MissionPlanningWaypoint::selected()
{
    setHighlighted(true);
}

void MissionPlanningWaypoint::deselected()
{
    setHighlighted(false);
}

void MissionPlanningWaypoint::setSelectionEnabled(bool selectionEnabled)
{
    if (selectionEnabled_ != selectionEnabled) {
        selectionEnabled_ = selectionEnabled;
        emit selectionEnabledChanged(selectionEnabled_);
    }
}

bool MissionPlanningWaypoint::selectionEnabled() const
{
    return selectionEnabled_;
}

void MissionPlanningWaypoint::setHighlighted(bool highlighted)
{
    if (highlighted_ != highlighted) {
        highlighted_ = highlighted;
        emit highlightChanged(highlighted_);
    }
}

bool MissionPlanningWaypoint::highlighted() const
{
    return highlighted_;
}

void MissionPlanningWaypoint::setDraggingEnabled(bool dragging)
{
    if (dragging != draggingEnabled_) {
        draggingEnabled_ = dragging;
        emit draggingEnabledChanged(draggingEnabled_);
    }
}

bool MissionPlanningWaypoint::draggingEnabled() const
{
    return draggingEnabled_;
}

void MissionPlanningWaypoint::dragStarted()
{
    emit dragStartedFromDrawing();
}

void MissionPlanningWaypoint::dragging(const QGeoCoordinate& dragLocation)
{
    emit draggingOccurredFromDrawing(dragLocation);
}

void MissionPlanningWaypoint::dragConfirmed(const QGeoCoordinate& dragLocation)
{
    emit dragConfirmedFromDrawing(dragLocation);
}

void MissionPlanningWaypoint::dragCancelled()
{
    emit dragCancelledFromDrawing();
}