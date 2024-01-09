/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <ExampleGeospatialSimpleWaypoint.h>
#include <ExampleSimpleWaypointConnector.h>
#include <QList>
#include <QObject>
#include <QTimer>

namespace LmCdl {
class I_SimpleWaypointConnector;
}

class ExampleMissionDomain : public QObject {
    Q_OBJECT
public:
    ExampleMissionDomain();
    ~ExampleMissionDomain();

    QList<ExampleGeospatialSimpleWaypoint*> waypoints() const;
    QList<LmCdl::I_SimpleWaypointConnector*> waypointConnectors() const;

private:
    Q_DISABLE_COPY(ExampleMissionDomain)

    class ConnectedWaypointRef {
    public:
        ConnectedWaypointRef(ExampleGeospatialSimpleWaypoint& waypoint,
            ExampleSimpleWaypointConnector& connectorEndingAtWaypoint,
            ExampleSimpleWaypointConnector& connectorStartingAtWaypoint);

        void setLabel(const QString& label);
        void setLocation(const QGeoCoordinate& location);

    private:
        ExampleGeospatialSimpleWaypoint& waypoint_;
        ExampleSimpleWaypointConnector& connectorEndingAtWaypoint_;
        ExampleSimpleWaypointConnector& connectorStartingAtWaypoint_;
    };

    struct DragInProgress {
        DragInProgress();
        explicit DragInProgress(const ExampleGeospatialSimpleWaypoint& waypointBeingDragged);

        QString label;
        QGeoCoordinate originalLocation;
    };

    void setupWaypointForHawaii(ExampleGeospatialSimpleWaypoint& waypoint, int id);
    void changeWaypointProperties();

    void setupConnectorExamples();
    void setupDirectionalConnectorExample();
    void setupOverlapConnectorExample();
    void setupAwayFromWaypointsConnectorExample();

    void updateAwayFromWaypointConnectorCoordinates();

    void setupDraggableWaypointExample();
    void connectDraggingForWaypoint(ExampleGeospatialSimpleWaypoint& waypoint,
        ExampleSimpleWaypointConnector& connectorEndingAtWaypoint,
        ExampleSimpleWaypointConnector& connectorStartingAtWaypoint);
    void initializeDragging(ExampleGeospatialSimpleWaypoint& waypoint);
    void dragWaypointAndConnectors(
        const QGeoCoordinate& dragCoordinate, ConnectedWaypointRef waypointAndConnectors) const;
    void completeDrag(const QGeoCoordinate& dragCoordinate, ConnectedWaypointRef waypointAndConnectors);
    void abortDrag(ConnectedWaypointRef waypointAndConnectors);

    QScopedPointer<ExampleGeospatialSimpleWaypoint> waypoint1_;
    QScopedPointer<ExampleGeospatialSimpleWaypoint> waypoint2_;
    QScopedPointer<ExampleGeospatialSimpleWaypoint> waypoint3_;
    QScopedPointer<ExampleGeospatialSimpleWaypoint> waypoint4_;
    QScopedPointer<ExampleGeospatialSimpleWaypoint> waypoint5_;
    QScopedPointer<ExampleGeospatialSimpleWaypoint> waypoint6_;

    QList<ExampleGeospatialSimpleWaypoint*> allWaypoints_;
    QList<LmCdl::I_SimpleWaypointConnector*> connectorsToDisplay_;

    QScopedPointer<ExampleSimpleWaypointConnector> directionalExampleConnectorWest_;
    QScopedPointer<ExampleSimpleWaypointConnector> directionalExampleConnectorEast_;

    QScopedPointer<ExampleSimpleWaypointConnector> overlapExampleConnectorTop_;
    QScopedPointer<ExampleSimpleWaypointConnector> overlapExampleConnectorBottom_;

    QScopedPointer<ExampleSimpleWaypointConnector> awayFromWaypointsExamplePrimaryConnector_;
    QScopedPointer<ExampleSimpleWaypointConnector> awayFromWaypointsExamplePerpendicularConnector_;
    QList<QGeoCoordinate> awayFromWaypointExampleAllVertices_;
    int awayFromWaypointExampleCurrentVertex_;

    QScopedPointer<ExampleGeospatialSimpleWaypoint> draggableWaypoint1_;
    QScopedPointer<ExampleGeospatialSimpleWaypoint> draggableWaypoint2_;
    QScopedPointer<ExampleGeospatialSimpleWaypoint> draggableWaypoint3_;
    QScopedPointer<ExampleSimpleWaypointConnector> draggableConnector12_;
    QScopedPointer<ExampleSimpleWaypointConnector> draggableConnector23_;
    QScopedPointer<ExampleSimpleWaypointConnector> draggableConnector31_;
    DragInProgress dragInProgress_;

    QTimer timerToChangeWaypointProperties_;
    bool displayNonDefaultWaypointProperties_;
};
