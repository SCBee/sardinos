#pragma once

#include <QList>
#include <QObject>
#include <QTimer>

#include <LmCdl/I_MissionDrawingApi.h>
#include <MissionPlanningWaypoint.h>
#include <MissionPlanningWaypointConnector.h>

namespace LmCdl
{
class I_SimpleWaypointConnector;
}

class MissionDomain : public QObject
{
    Q_OBJECT
public:
    MissionDomain(){}
    ~MissionDomain(){}

    QList<MissionPlanningWaypoint*> waypoints() const;
    QList<MissionPlanningWaypointConnector*> waypointConnectors() const;

    void setPath(QList<QGeoCoordinate> coordinates);

private:
    Q_DISABLE_COPY(MissionDomain)

    class ConnectedWaypointRef
    {
    public:
        ConnectedWaypointRef(
            MissionPlanningWaypoint& waypoint,
            MissionPlanningWaypointConnector& connectorEndingAtWaypoint,
            MissionPlanningWaypointConnector& connectorStartingAtWaypoint);

        void setLabel(const QString& label);
        void setLocation(const QGeoCoordinate& location);

    private:
        MissionPlanningWaypoint& waypoint_;
        MissionPlanningWaypointConnector& connectorEndingAtWaypoint_;
        MissionPlanningWaypointConnector& connectorStartingAtWaypoint_;
    };

    struct DragInProgress
    {
        DragInProgress();
        explicit DragInProgress(
            const MissionPlanningWaypoint& waypointBeingDragged);

        QString label;
        QGeoCoordinate originalLocation;
    };

    void setupWaypoints(QList<QGeoCoordinate> coordinates);
    void setupConnectors();

    void connectDraggingForWaypoints();
    void connectDraggingForWaypoint(MissionPlanningWaypoint& waypoint,  MissionPlanningWaypointConnector& connectorEndingAtWaypoint, MissionPlanningWaypointConnector& connectorStartingAtWaypoint);
    
    void initializeDragging(MissionPlanningWaypoint& waypoint);
    
    void dragWaypointAndConnectors( const QGeoCoordinate& dragCoordinate, ConnectedWaypointRef waypointAndConnectors) const;
    
    void completeDrag(const QGeoCoordinate& dragCoordinate, ConnectedWaypointRef waypointAndConnectors);
    
    void abortDrag(ConnectedWaypointRef waypointAndConnectors);

    std::vector<std::pair<MissionPlanningWaypoint*, QList<MissionPlanningWaypointConnector*>>> waypoints_;
    QScopedPointer<MissionPlanningWaypoint> waypoint_;
    QScopedPointer<MissionPlanningWaypointConnector> connector_;

    DragInProgress dragInProgress_;
};
