#include <QColor>
#include <utility>

#include <MissionDomain.h>
#include <Sardinos.h>

QList<Waypoint*> MissionDomain::waypoints() const
{
    auto waypoints = QList<Waypoint*>();
    for (const auto& pair : waypoints_) {
        waypoints.append(pair.first);
    }

    return waypoints;
}

QList<LmCdl::I_SimpleWaypointConnector*> MissionDomain::waypointConnectors()
    const
{
    auto connectors = QList<LmCdl::I_SimpleWaypointConnector*>();
    for (const auto& pair : waypoints_) {
        for (auto c : pair.second)
            connectors.append(c);
    }

    return connectors;
}

void MissionDomain::setPath(QList<QGeoCoordinate> coordinates)
{
    setupWaypoints(std::move(coordinates));
    setupConnectors();
    connectDraggingForWaypoints();
}

void MissionDomain::setupWaypoints(QList<QGeoCoordinate> coordinates)
{
    waypoints_.clear();
    for (auto i = 0; i < coordinates.size(); i++) {
        auto waypoint = new Waypoint();
        waypoint->setColor(Qt::darkCyan);
        waypoint->setShape(LmCdl::I_GeospatialSimpleWaypoint::Shape::Circle);
        waypoint->setLabel(QString(std::to_string(i).c_str()));
        waypoint->setLocation(coordinates[i]);
        waypoint->setSelectionEnabled(true);
        waypoint->setDraggingEnabled(true);
        waypoint->setVisible(true);
        waypoints_.emplace_back(waypoint, QList<WaypointConnector*>());
    }
}

void MissionDomain::setupConnectors()
{
    for (auto i = 0; i < waypoints_.size() - 1; i++) {
        auto p1        = waypoints_[i].first;
        auto p2        = waypoints_[i + 1].first;
        auto connector = new WaypointConnector();
        connector->setStartLocation(p1->location());
        connector->setEndLocation(p2->location());
        connector->setColor(Qt::darkCyan);
        connector->setVisible(true);
        connector->setDirectionalIndicatorVisible(true);
        waypoints_[i].second.append(connector);
        waypoints_[i + 1].second.append(connector);
    }
}

void MissionDomain::connectDraggingForWaypoints()
{
    for (auto i = 0; i < waypoints_.size(); i++) {
        if (i == 0)
            connectDraggingForWaypoint(*(waypoints_[i].first),
                                       *new WaypointConnector(),
                                       *(waypoints_[i]).second[0]);
        else if (i == waypoints_.size() - 1)
            connectDraggingForWaypoint(*(waypoints_[i].first),
                                       *(waypoints_[i]).second[0],
                                       *new WaypointConnector());
        else
            connectDraggingForWaypoint(*(waypoints_[i].first),
                                       *(waypoints_[i]).second[0],
                                       *(waypoints_[i]).second[1]);
    }
}

void MissionDomain::connectDraggingForWaypoint(
    Waypoint& waypoint,
    WaypointConnector& connectorEndingAtWaypoint,
    WaypointConnector& connectorStartingAtWaypoint)
{
    ConnectedWaypointRef connectedWaypoint(
        waypoint, connectorEndingAtWaypoint, connectorStartingAtWaypoint);

    connect(&waypoint,
            &Waypoint::dragStartedFromDrawing,
            this,
            [this, &waypoint]() { initializeDragging(waypoint); });

    connect(&waypoint,
            &Waypoint::draggingOccurredFromDrawing,
            this,
            [connectedWaypoint](const QGeoCoordinate& dragLocation)
            { dragWaypointAndConnectors(dragLocation, connectedWaypoint); });

    connect(&waypoint,
            &Waypoint::dragConfirmedFromDrawing,
            this,
            [this, connectedWaypoint](const QGeoCoordinate& dragLocation)
            { completeDrag(dragLocation, connectedWaypoint); });

    connect(&waypoint,
            &Waypoint::dragCancelledFromDrawing,
            this,
            [this, connectedWaypoint]() { abortDrag(connectedWaypoint); });
}

void MissionDomain::initializeDragging(Waypoint& waypoint)
{
    dragInProgress_ = DragInProgress(waypoint);

    waypoint.setLabel("DRAGGING");
}

void MissionDomain::dragWaypointAndConnectors(
    const QGeoCoordinate& dragCoordinate,
    ConnectedWaypointRef waypointAndConnectors)
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

MissionDomain::DragInProgress::DragInProgress() = default;

MissionDomain::DragInProgress::DragInProgress(
    const Waypoint& waypointBeingDragged)
    : label(waypointBeingDragged.label())
    , originalLocation(waypointBeingDragged.location())
{
}

MissionDomain::ConnectedWaypointRef::ConnectedWaypointRef(
    Waypoint& waypoint,
    WaypointConnector& connectorEndingAtWaypoint,
    WaypointConnector& connectorStartingAtWaypoint)
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

void MissionDomain::startMission()
{
    for (auto& waypoint : waypoints_) {
        waypoint.first->setDraggingEnabled(false);
        waypoint.first->setSelectionEnabled(false);
        waypoint.first->setColor(Qt::darkGreen);
        for (auto& j : waypoint.second) {
            j->setColor(Qt::darkGreen);
        }
    }
}