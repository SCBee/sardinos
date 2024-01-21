#include <QColor>

#include <MissionDomain.h>

QList<MissionPlanningWaypoint*> MissionDomain::waypoints() const
{
    auto waypoints = QList<MissionPlanningWaypoint*>();
    for (auto pair : waypoints_) {
        waypoints.append(pair.first);
    }

    return waypoints;
}

QList<MissionPlanningWaypointConnector*> MissionDomain::waypointConnectors()
    const
{
    auto connectors = QList<MissionPlanningWaypointConnector*>();
    for (auto pair : waypoints_) {
        connectors.append(pair.second);
    }

    return connectors;
}

void MissionDomain::setPath(QList<QGeoCoordinate> coordinates)
{
    setupWaypoints(coordinates);
    setupConnectors();
    connectDraggingForWaypoints();
}

void MissionDomain::setupWaypoints(QList<QGeoCoordinate> coordinates)
{
    waypoints_.clear();
    for (auto i = 0; i < coordinates.size(); i++) {
        //waypoint_.reset(new MissionPlanningWaypoint);
        // auto waypoint = new MissionPlanningWaypoint();
        waypoint_->setColor(QColor(255, 255, 255));
        waypoint_->setShape(LmCdl::I_GeospatialSimpleWaypoint::Shape::Triangle);
        waypoint_->setLabel(QString(i));
        waypoint_->setLocation(coordinates[i]);
        waypoint_->setSelectionEnabled(true);
        waypoint_->setDraggingEnabled(true);
        waypoint_->setVisible(true);
        waypoints_.push_back(std::make_pair(
            waypoint_.data(), QList<MissionPlanningWaypointConnector*>()));
    }
}

void MissionDomain::setupConnectors()
{
    for (auto i = 0; i < waypoints_.size() - 1; i++) {
        auto p1 = waypoints_[i].first;
        auto p2 = waypoints_[i + 1].first;
        //connector_.reset(new MissionPlanningWaypointConnector);
        // auto connector = new MissionPlanningWaypointConnector();
        connector_->setStartLocation(p1->location());
        connector_->setEndLocation(p2->location());
        connector_->setColor(QColor(255, 0, 0));
        connector_->setVisible(true);
        connector_->setDirectionalIndicatorVisible(true);
        waypoints_[i].second.append(connector_.data());
        waypoints_[i + 1].second.append(connector_.data());
    }
}

void MissionDomain::connectDraggingForWaypoints()
{
    for (auto i = 0; i < waypoints_.size(); i++) {
        if (i == 0)
            connectDraggingForWaypoint(*(waypoints_[i].first),
                                       *new MissionPlanningWaypointConnector(),
                                       *(waypoints_[i]).second[0]);
        else if (i == waypoints_.size() - 1)
            connectDraggingForWaypoint(*(waypoints_[i].first),
                                       *(waypoints_[i]).second[0],
                                       *new MissionPlanningWaypointConnector());
        else
            connectDraggingForWaypoint(*(waypoints_[i].first),
                                       *(waypoints_[i]).second[0],
                                       *(waypoints_[i]).second[1]);
    }
}

void MissionDomain::connectDraggingForWaypoint(
    MissionPlanningWaypoint& waypoint,
    MissionPlanningWaypointConnector& connectorEndingAtWaypoint,
    MissionPlanningWaypointConnector& connectorStartingAtWaypoint)
{
    ConnectedWaypointRef connectedWaypoint(
        waypoint, connectorEndingAtWaypoint, connectorStartingAtWaypoint);

    connect(&waypoint,
            &MissionPlanningWaypoint::dragStartedFromDrawing,
            this,
            [this, &waypoint]() { initializeDragging(waypoint); });

    connect(&waypoint,
            &MissionPlanningWaypoint::draggingOccuredFromDrawing,
            this,
            [this, connectedWaypoint](const QGeoCoordinate& dragLocation)
            { dragWaypointAndConnectors(dragLocation, connectedWaypoint); });

    connect(&waypoint,
            &MissionPlanningWaypoint::dragConfirmedFromDrawing,
            this,
            [this, connectedWaypoint](const QGeoCoordinate& dragLocation)
            { completeDrag(dragLocation, connectedWaypoint); });

    connect(&waypoint,
            &MissionPlanningWaypoint::dragCancelledFromDrawing,
            this,
            [this, connectedWaypoint]() { abortDrag(connectedWaypoint); });
}

void MissionDomain::initializeDragging(MissionPlanningWaypoint& waypoint)
{
    dragInProgress_ = DragInProgress(waypoint);

    waypoint.setLabel("DRAGGING");
}

void MissionDomain::dragWaypointAndConnectors(
    const QGeoCoordinate& dragCoordinate,
    ConnectedWaypointRef waypointAndConnectors) const
{
    waypointAndConnectors.setLocation(dragCoordinate);
}

void MissionDomain::completeDrag(const QGeoCoordinate& dragCoordinate,
                                 ConnectedWaypointRef waypointAndConnectors)
{
    waypointAndConnectors.setLocation(dragCoordinate);
    waypointAndConnectors.setLabel(dragInProgress_.label);

    dragInProgress_ = DragInProgress();
}

void MissionDomain::abortDrag(ConnectedWaypointRef waypointAndConnectors)
{
    waypointAndConnectors.setLocation(dragInProgress_.originalLocation);
    waypointAndConnectors.setLabel(dragInProgress_.label);

    dragInProgress_ = DragInProgress();
}

MissionDomain::DragInProgress::DragInProgress() {}

MissionDomain::DragInProgress::DragInProgress(
    const MissionPlanningWaypoint& waypointBeingDragged)
    : label(waypointBeingDragged.label())
    , originalLocation(waypointBeingDragged.location())
{
}

MissionDomain::ConnectedWaypointRef::ConnectedWaypointRef(
    MissionPlanningWaypoint& waypoint,
    MissionPlanningWaypointConnector& connectorEndingAtWaypoint,
    MissionPlanningWaypointConnector& connectorStartingAtWaypoint)
    : waypoint_(waypoint)
    , connectorEndingAtWaypoint_(connectorEndingAtWaypoint)
    , connectorStartingAtWaypoint_(connectorStartingAtWaypoint)
{
}

void MissionDomain::ConnectedWaypointRef::setLabel(const QString& label)
{
    waypoint_.setLabel(label);
}

void MissionDomain::ConnectedWaypointRef::setLocation(
    const QGeoCoordinate& location)
{
    waypoint_.setLocation(location);
    connectorEndingAtWaypoint_.setEndLocation(location);
    connectorStartingAtWaypoint_.setStartLocation(location);
}