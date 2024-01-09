/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <ExampleMissionDomain.h>

namespace {
const double LATITUDE_IN_HAWAII = 21.40375;
const double LONGITUDE_IN_HAWAII = -157.9866833;
}

ExampleMissionDomain::ExampleMissionDomain()
    : waypoint1_(new ExampleGeospatialSimpleWaypoint)
    , waypoint2_(new ExampleGeospatialSimpleWaypoint)
    , waypoint3_(new ExampleGeospatialSimpleWaypoint)
    , waypoint4_(new ExampleGeospatialSimpleWaypoint)
    , waypoint5_(new ExampleGeospatialSimpleWaypoint)
    , waypoint6_(new ExampleGeospatialSimpleWaypoint)
    , allWaypoints_()
    , timerToChangeWaypointProperties_()
    , displayNonDefaultWaypointProperties_(true)
{
    setupWaypointForHawaii(*waypoint1_, 1);
    setupWaypointForHawaii(*waypoint2_, 2);
    setupWaypointForHawaii(*waypoint3_, 3);
    setupWaypointForHawaii(*waypoint4_, 4);
    setupWaypointForHawaii(*waypoint5_, 5);
    setupWaypointForHawaii(*waypoint6_, 6);

    // To show relative z-order changes, place the last waypoint over top of the first waypoint.
    waypoint6_->setLocation(waypoint1_->location());

    setupConnectorExamples();

    setupDraggableWaypointExample();

    timerToChangeWaypointProperties_.setInterval(5000); // 5 seconds
    connect(&timerToChangeWaypointProperties_, &QTimer::timeout, this, &ExampleMissionDomain::changeWaypointProperties);
    timerToChangeWaypointProperties_.start();
}

ExampleMissionDomain::~ExampleMissionDomain() { }

QList<ExampleGeospatialSimpleWaypoint*> ExampleMissionDomain::waypoints() const { return allWaypoints_; }

QList<LmCdl::I_SimpleWaypointConnector*> ExampleMissionDomain::waypointConnectors() const
{
    return connectorsToDisplay_;
}

void ExampleMissionDomain::setupWaypointForHawaii(ExampleGeospatialSimpleWaypoint& waypoint, int id)
{
    waypoint.setLocation(QGeoCoordinate(LATITUDE_IN_HAWAII, LONGITUDE_IN_HAWAII + (id / 10.0), 7000));
    waypoint.setLabel(QString("%1").arg(id));
    allWaypoints_.append(&waypoint);
}

void ExampleMissionDomain::changeWaypointProperties()
{
    if (displayNonDefaultWaypointProperties_) {
        waypoint1_->setLocation(QGeoCoordinate(LATITUDE_IN_HAWAII, LONGITUDE_IN_HAWAII, 7000));
        waypoint2_->setShape(LmCdl::I_GeospatialSimpleWaypoint::Shape::InvertedTriangle);
        waypoint3_->setLabel("0");
        waypoint3_->setShape(LmCdl::I_GeospatialSimpleWaypoint::Shape::House);
        waypoint4_->setVisible(false);
        waypoint5_->setColor(QColor("magenta"));
        waypoint6_->setRelativeZOrder(0);
    } else {
        waypoint1_->setLocation(QGeoCoordinate(LATITUDE_IN_HAWAII, LONGITUDE_IN_HAWAII + 0.1, 7000));
        waypoint2_->setShape(LmCdl::I_GeospatialSimpleWaypoint::Shape::Circle);
        waypoint3_->setLabel("3");
        waypoint3_->setShape(LmCdl::I_GeospatialSimpleWaypoint::Shape::Circle);
        waypoint4_->setVisible(true);
        waypoint5_->setColor(QColor());
        waypoint6_->setRelativeZOrder(2);
    }
    displayNonDefaultWaypointProperties_ = !displayNonDefaultWaypointProperties_;
}

void ExampleMissionDomain::setupConnectorExamples()
{
    setupDirectionalConnectorExample();
    setupOverlapConnectorExample();
    setupAwayFromWaypointsConnectorExample();
}

/**
 * Add two outgoing connectors to demonstrate the direction of travel indicators
 */
void ExampleMissionDomain::setupDirectionalConnectorExample()
{
    directionalExampleConnectorWest_.reset(new ExampleSimpleWaypointConnector());
    directionalExampleConnectorWest_->setStartLocation(waypoint2_->location());
    directionalExampleConnectorWest_->setEndLocation(waypoint6_->location());
    directionalExampleConnectorWest_->setVisible(true);
    directionalExampleConnectorWest_->setDirectionalIndicatorVisible(true);

    directionalExampleConnectorEast_.reset(new ExampleSimpleWaypointConnector());
    directionalExampleConnectorEast_->setStartLocation(waypoint2_->location());
    directionalExampleConnectorEast_->setEndLocation(waypoint3_->location());
    directionalExampleConnectorEast_->setVisible(true);
    directionalExampleConnectorEast_->setDirectionalIndicatorVisible(true);

    connectorsToDisplay_.append(directionalExampleConnectorWest_.data());
    connectorsToDisplay_.append(directionalExampleConnectorEast_.data());
}

/**
 *  Put two connectors at the same location to demonstrate overlap. Use different colors and visibility so we can see
 *  both, just not at the same time.
 */
void ExampleMissionDomain::setupOverlapConnectorExample()
{
    overlapExampleConnectorTop_.reset(new ExampleSimpleWaypointConnector());
    overlapExampleConnectorTop_->setStartLocation(waypoint4_->location());
    overlapExampleConnectorTop_->setEndLocation(waypoint5_->location());
    overlapExampleConnectorTop_->setColor(QColor(255, 0, 255, 180));
    overlapExampleConnectorTop_->setRelativeZOrder(5);
    overlapExampleConnectorTop_->setVisible(waypoint4_->visible());
    overlapExampleConnectorTop_->setDirectionalIndicatorVisible(true);

    connect(waypoint4_.data(), &LmCdl::I_GeospatialSimpleWaypoint::visibilityChanged,
        overlapExampleConnectorTop_.data(), &ExampleSimpleWaypointConnector::setVisible);

    overlapExampleConnectorBottom_.reset(new ExampleSimpleWaypointConnector());
    overlapExampleConnectorBottom_->setStartLocation(waypoint4_->location());
    overlapExampleConnectorBottom_->setEndLocation(waypoint5_->location());
    overlapExampleConnectorBottom_->setColor(QColor(192, 192, 192));
    overlapExampleConnectorBottom_->setRelativeZOrder(3);
    overlapExampleConnectorBottom_->setVisible(true);
    overlapExampleConnectorBottom_->setDirectionalIndicatorVisible(true);

    connectorsToDisplay_.append(overlapExampleConnectorTop_.data());
    connectorsToDisplay_.append(overlapExampleConnectorBottom_.data());
}

/**
 * Demonstrate that waypoint connectors don't need to be used with waypoints. Add some spinning to exercise location
 * changing.
 */
void ExampleMissionDomain::setupAwayFromWaypointsConnectorExample()
{
    awayFromWaypointsExamplePrimaryConnector_.reset(new ExampleSimpleWaypointConnector());
    awayFromWaypointsExamplePrimaryConnector_->setColor(Qt::blue);
    awayFromWaypointsExamplePrimaryConnector_->setVisible(true);
    awayFromWaypointsExamplePrimaryConnector_->setDirectionalIndicatorVisible(false);

    awayFromWaypointsExamplePerpendicularConnector_.reset(new ExampleSimpleWaypointConnector());
    awayFromWaypointsExamplePerpendicularConnector_->setColor(Qt::red);
    awayFromWaypointsExamplePerpendicularConnector_->setVisible(true);
    awayFromWaypointsExamplePerpendicularConnector_->setDirectionalIndicatorVisible(false);

    awayFromWaypointExampleCurrentVertex_ = 0;
    awayFromWaypointExampleAllVertices_ = {
        QGeoCoordinate(21 + 20.1124461 / 60, -(157 + 30.5897094 / 60), 270),
        QGeoCoordinate(21 + 17.9516294 / 60, -(157 + 28.2960102 / 60), 290),
        QGeoCoordinate(21 + 15.7908127 / 60, -(157 + 26.0023109 / 60), 310),
        QGeoCoordinate(21 + 13.5435548 / 60, -(157 + 28.2960102 / 60), 330),
        QGeoCoordinate(21 + 11.2962968 / 60, -(157 + 30.5897094 / 60), 350),
        QGeoCoordinate(21 + 13.5435548 / 60, -(157 + 33.1443586 / 60), 370),
        QGeoCoordinate(21 + 15.7908127 / 60, -(157 + 35.6990077 / 60), 390),
        QGeoCoordinate(21 + 17.9516294 / 60, -(157 + 33.1443586 / 60), 410),
    };
    updateAwayFromWaypointConnectorCoordinates();

    connect(&timerToChangeWaypointProperties_, &QTimer::timeout, this,
        &ExampleMissionDomain::updateAwayFromWaypointConnectorCoordinates);

    connectorsToDisplay_.append(awayFromWaypointsExamplePrimaryConnector_.data());
    connectorsToDisplay_.append(awayFromWaypointsExamplePerpendicularConnector_.data());
}

void ExampleMissionDomain::updateAwayFromWaypointConnectorCoordinates()
{
    const int stepSize = awayFromWaypointExampleAllVertices_.size() / 4;

    awayFromWaypointsExamplePrimaryConnector_->setStartLocation(
        awayFromWaypointExampleAllVertices_.at(awayFromWaypointExampleCurrentVertex_));
    awayFromWaypointsExamplePrimaryConnector_->setEndLocation(awayFromWaypointExampleAllVertices_.at(
        (awayFromWaypointExampleCurrentVertex_ + 2 * stepSize) % awayFromWaypointExampleAllVertices_.size()));

    awayFromWaypointsExamplePerpendicularConnector_->setStartLocation(awayFromWaypointExampleAllVertices_.at(
        (awayFromWaypointExampleCurrentVertex_ + 1 * stepSize) % awayFromWaypointExampleAllVertices_.size()));
    awayFromWaypointsExamplePerpendicularConnector_->setEndLocation(awayFromWaypointExampleAllVertices_.at(
        (awayFromWaypointExampleCurrentVertex_ + 3 * stepSize) % awayFromWaypointExampleAllVertices_.size()));

    awayFromWaypointExampleCurrentVertex_
        = (awayFromWaypointExampleCurrentVertex_ + 1) % awayFromWaypointExampleAllVertices_.size();
}

void ExampleMissionDomain::setupDraggableWaypointExample()
{
    draggableWaypoint1_.reset(new ExampleGeospatialSimpleWaypoint());
    draggableWaypoint1_->setColor(QColor(255, 0, 0));
    draggableWaypoint1_->setLabel("DRAG ME Red");
    draggableWaypoint1_->setLocation(QGeoCoordinate(21 + 28.7042667 / 60, -(157 + 26.0928500 / 60), 625));
    draggableWaypoint1_->setSelectionEnabled(true);
    draggableWaypoint1_->setDraggingEnabled(true);
    draggableWaypoint1_->setVisible(true);
    allWaypoints_.append(draggableWaypoint1_.data());

    draggableWaypoint2_.reset(new ExampleGeospatialSimpleWaypoint());
    draggableWaypoint2_->setColor(QColor(0, 255, 0));
    draggableWaypoint2_->setLabel("DRAG ME Green");
    draggableWaypoint2_->setLocation(QGeoCoordinate(21 + 36.8912000 / 60, -(157 + 31.6577417 / 60), 625));
    draggableWaypoint2_->setSelectionEnabled(true);
    draggableWaypoint2_->setDraggingEnabled(true);
    draggableWaypoint2_->setVisible(true);
    allWaypoints_.append(draggableWaypoint2_.data());

    draggableWaypoint3_.reset(new ExampleGeospatialSimpleWaypoint());
    draggableWaypoint3_->setColor(QColor(0, 0, 255));
    draggableWaypoint3_->setLabel("DRAG ME Blue");
    draggableWaypoint3_->setLocation(QGeoCoordinate(21 + 28.7042667 / 60, -(157 + 37.2226333 / 60), 625));
    draggableWaypoint3_->setSelectionEnabled(true);
    draggableWaypoint3_->setDraggingEnabled(true);
    draggableWaypoint3_->setVisible(true);
    allWaypoints_.append(draggableWaypoint3_.data());

    draggableConnector12_.reset(new ExampleSimpleWaypointConnector());
    draggableConnector12_->setStartLocation(draggableWaypoint1_->location());
    draggableConnector12_->setEndLocation(draggableWaypoint2_->location());
    draggableConnector12_->setColor(QColor(255, 255, 0));
    draggableConnector12_->setVisible(true);
    draggableConnector12_->setDirectionalIndicatorVisible(true);
    connectorsToDisplay_.append(draggableConnector12_.data());

    draggableConnector23_.reset(new ExampleSimpleWaypointConnector());
    draggableConnector23_->setStartLocation(draggableWaypoint2_->location());
    draggableConnector23_->setEndLocation(draggableWaypoint3_->location());
    draggableConnector23_->setColor(QColor(0, 255, 255));
    draggableConnector23_->setVisible(true);
    draggableConnector23_->setDirectionalIndicatorVisible(true);
    connectorsToDisplay_.append(draggableConnector23_.data());

    draggableConnector31_.reset(new ExampleSimpleWaypointConnector());
    draggableConnector31_->setStartLocation(draggableWaypoint3_->location());
    draggableConnector31_->setEndLocation(draggableWaypoint1_->location());
    draggableConnector31_->setColor(QColor(255, 0, 255));
    draggableConnector31_->setVisible(true);
    draggableConnector31_->setDirectionalIndicatorVisible(true);
    connectorsToDisplay_.append(draggableConnector31_.data());

    connectDraggingForWaypoint(*draggableWaypoint1_, *draggableConnector31_, *draggableConnector12_);
    connectDraggingForWaypoint(*draggableWaypoint2_, *draggableConnector12_, *draggableConnector23_);
    connectDraggingForWaypoint(*draggableWaypoint3_, *draggableConnector23_, *draggableConnector31_);
}

void ExampleMissionDomain::connectDraggingForWaypoint(ExampleGeospatialSimpleWaypoint& waypoint,
    ExampleSimpleWaypointConnector& connectorEndingAtWaypoint,
    ExampleSimpleWaypointConnector& connectorStartingAtWaypoint)
{
    ConnectedWaypointRef connectedWaypoint(waypoint, connectorEndingAtWaypoint, connectorStartingAtWaypoint);

    connect(&waypoint, &ExampleGeospatialSimpleWaypoint::dragStartedFromDrawing, this,
        [this, &waypoint]() { initializeDragging(waypoint); });

    connect(&waypoint, &ExampleGeospatialSimpleWaypoint::draggingOccuredFromDrawing, this,
        [this, connectedWaypoint](
            const QGeoCoordinate& dragLocation) { dragWaypointAndConnectors(dragLocation, connectedWaypoint); });

    connect(&waypoint, &ExampleGeospatialSimpleWaypoint::dragConfirmedFromDrawing, this,
        [this, connectedWaypoint](
            const QGeoCoordinate& dragLocation) { completeDrag(dragLocation, connectedWaypoint); });

    connect(&waypoint, &ExampleGeospatialSimpleWaypoint::dragCancelledFromDrawing, this,
        [this, connectedWaypoint]() { abortDrag(connectedWaypoint); });
}

void ExampleMissionDomain::initializeDragging(ExampleGeospatialSimpleWaypoint& waypoint)
{
    dragInProgress_ = DragInProgress(waypoint);

    waypoint.setLabel("DRAGGING");
}

void ExampleMissionDomain::dragWaypointAndConnectors(
    const QGeoCoordinate& dragCoordinate, ConnectedWaypointRef waypointAndConnectors) const
{
    waypointAndConnectors.setLocation(dragCoordinate);
}

void ExampleMissionDomain::completeDrag(
    const QGeoCoordinate& dragCoordinate, ConnectedWaypointRef waypointAndConnectors)
{
    waypointAndConnectors.setLocation(dragCoordinate);
    waypointAndConnectors.setLabel(dragInProgress_.label);

    dragInProgress_ = DragInProgress();
}

void ExampleMissionDomain::abortDrag(ConnectedWaypointRef waypointAndConnectors)
{
    waypointAndConnectors.setLocation(dragInProgress_.originalLocation);
    waypointAndConnectors.setLabel(dragInProgress_.label);

    dragInProgress_ = DragInProgress();
}

ExampleMissionDomain::DragInProgress::DragInProgress() { }

ExampleMissionDomain::DragInProgress::DragInProgress(const ExampleGeospatialSimpleWaypoint& waypointBeingDragged)
    : label(waypointBeingDragged.label())
    , originalLocation(waypointBeingDragged.location())
{
}

ExampleMissionDomain::ConnectedWaypointRef::ConnectedWaypointRef(ExampleGeospatialSimpleWaypoint& waypoint,
    ExampleSimpleWaypointConnector& connectorEndingAtWaypoint,
    ExampleSimpleWaypointConnector& connectorStartingAtWaypoint)
    : waypoint_(waypoint)
    , connectorEndingAtWaypoint_(connectorEndingAtWaypoint)
    , connectorStartingAtWaypoint_(connectorStartingAtWaypoint)
{
}

void ExampleMissionDomain::ConnectedWaypointRef::setLabel(const QString& label) { waypoint_.setLabel(label); }

void ExampleMissionDomain::ConnectedWaypointRef::setLocation(const QGeoCoordinate& location)
{
    waypoint_.setLocation(location);
    connectorEndingAtWaypoint_.setEndLocation(location);
    connectorStartingAtWaypoint_.setStartLocation(location);
}
