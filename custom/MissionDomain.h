#pragma once

#include <QList>
#include <QObject>
#include <QTimer>

#include <LmCdl/I_MissionDrawingApi.h>
#include <Waypoint.h>
#include <WaypointConnector.h>

namespace LmCdl
{
    class I_SimpleWaypointConnector;
}

class MissionDomain : public QObject
{
    Q_OBJECT
public:
    MissionDomain()           = default;
    ~MissionDomain() override = default;

    void startMission();

    [[nodiscard]] QList<Waypoint*> waypoints() const;
    [[nodiscard]] QList<LmCdl::I_SimpleWaypointConnector*> waypointConnectors()
        const;

    void setPath(QList<QGeoCoordinate> coordinates);

private:
    Q_DISABLE_COPY(MissionDomain)

    class ConnectedWaypointRef
    {
    public:
        ConnectedWaypointRef(
            Waypoint& waypoint,
                             WaypointConnector& connectorEndingAtWaypoint,
                             WaypointConnector& connectorStartingAtWaypoint);

        void setLabel(const QString& label);
        void setLocation(const QGeoCoordinate& location);

    private:
        Waypoint& waypoint_;
        WaypointConnector& connectorEndingAtWaypoint_;
        WaypointConnector& connectorStartingAtWaypoint_;
    };

    struct DragInProgress
    {
        DragInProgress();
        explicit DragInProgress(
            const Waypoint& waypointBeingDragged);

        QString label;
        QGeoCoordinate originalLocation;
    };

    void setupWaypoints(QList<QGeoCoordinate> coordinates);
    void setupConnectors();

    void connectDraggingForWaypoints();
    void connectDraggingForWaypoint(
        Waypoint& waypoint,
        WaypointConnector& connectorEndingAtWaypoint,
        WaypointConnector& connectorStartingAtWaypoint);

    void initializeDragging(Waypoint& waypoint);

    static void dragWaypointAndConnectors(
        const QGeoCoordinate& dragCoordinate,
        ConnectedWaypointRef waypointAndConnectors);

    void completeDrag(const QGeoCoordinate& dragCoordinate,
                      ConnectedWaypointRef waypointAndConnectors);

    void abortDrag(ConnectedWaypointRef waypointAndConnectors);

    std::vector<std::pair<Waypoint*,
                          QList<WaypointConnector*>>>
        waypoints_;
    DragInProgress dragInProgress_;
};
