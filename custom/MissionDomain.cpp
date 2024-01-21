#include <QColor>

#include <MissionDomain.h>

QList<MissionPlanningWaypoint*> MissionDomain::waypoints() const
{
    return waypoints_;
}

QList<LmCdl::I_SimpleWaypointConnector*> MissionDomain::waypointConnectors() const
{
    return connectors_;
}

void MissionDomain::setPath(QList<QGeoCoordinate> coordinates)
{
    setupWaypoints(coordinates);
    setupConnectors(coordinates);
}

void MissionDomain::setupWaypoints(QList<QGeoCoordinate> coordinates)
{
    waypoints_.clear();
    for (auto i = 0; i < coordinates.size(); i++) {
        auto waypoint = new MissionPlanningWaypoint();
        waypoint->setColor(QColor(255, 255, 255));
        waypoint->setLabel(QString(i));
        waypoint->setLocation(coordinates[i]);
        waypoint->setSelectionEnabled(true);
        waypoint->setDraggingEnabled(true);
        waypoint->setVisible(true);
        waypoints_.append(waypoint);
    }
}

void MissionDomain::setupConnectors(QList<QGeoCoordinate> coordinates)
{
    connectors_.clear();
    for (auto i = 0; i < coordinates.size() - 1; i++) {
        auto p1 = coordinates[i];
        auto p2 = coordinates[i + 1];
        auto connector = new MissionPlanningWaypointConnector();
        connector->setStartLocation(p1);
        connector->setEndLocation(p2);
        connector->setColor(QColor(255, 0, 0));
        connector->setVisible(true);
        connector->setDirectionalIndicatorVisible(true);
        connectors_.append(connector);
    }
}

// void MissionDomain::connectDraggingForWaypoint(
//     MissionPlanningWaypoint& waypoint,
//     MissionPlanningWaypointConnector& connectorEndingAtWaypoint,
//     MissionPlanningWaypointConnector& connectorStartingAtWaypoint)
// {
//     ConnectedWaypointRef connectedWaypoint(
//         waypoint, connectorEndingAtWaypoint, connectorStartingAtWaypoint);

//     connect(&waypoint,
//             &MissionPlanningWaypoint::dragStartedFromDrawing,
//             this,
//             [this, &waypoint]() { initializeDragging(waypoint); });

//     connect(&waypoint,
//             &MissionPlanningWaypoint::draggingOccuredFromDrawing,
//             this,
//             [this, connectedWaypoint](const QGeoCoordinate& dragLocation)
//             { dragWaypointAndConnectors(dragLocation, connectedWaypoint); });

//     connect(&waypoint,
//             &MissionPlanningWaypoint::dragConfirmedFromDrawing,
//             this,
//             [this, connectedWaypoint](const QGeoCoordinate& dragLocation)
//             { completeDrag(dragLocation, connectedWaypoint); });

//     connect(&waypoint,
//             &MissionPlanningWaypoint::dragCancelledFromDrawing,
//             this,
//             [this, connectedWaypoint]() { abortDrag(connectedWaypoint); });
// }

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
