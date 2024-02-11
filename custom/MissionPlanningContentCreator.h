#pragma once

#include <QList>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>
#include <vector>

#include <BoundingBox.h>
#include <Drone.h>
#include <FlightPather.h>
#include <ImageProcessor.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_MissionDrawingApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_RouteApi.h>
#include <LmCdl/I_TrackDrawingApi.h>
#include <LmCdl/I_TrackVisualization.h>
#include <LmCdl/I_UserNotification.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_GroundElevationApi.h>
#include <LmCdl/I_VideoStreamApiCollection.h>
#include <LmCdl/UniqueIdentifier.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiMilStdCode.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <MissionDomain.h>
#include <MissionPlanningDrawing.h>
#include <MissionPlanningLine.h>
#include <MissionPlanningPolygon.h>
#include <MissionPlanningWaypoint.h>
#include <MissionPlanningWaypointConnector.h>
#include <qgeocoordinate.h>

namespace LmCdl
{
class I_VcsiMapExtensionApi;

class I_PointOfInterestApi;

class I_VcsiApplicationApi;
}  // namespace LmCdl

class MissionPlanningContentCreator : public QObject
{
    Q_OBJECT

public:
    enum State
    {
        STARTUP,
        CanGetFlightPath,
        CannotGetFlightPath,
        CanRunMission,
        CanCancelMission,
    };

    enum Severity
    {
        Message,
        Continue,
        Warning,
        Danger
    };

    MissionPlanningContentCreator(
        LmCdl::I_VcsiMapExtensionApi& mapApi,
        LmCdl::I_PointOfInterestApi& poiApi,
        LmCdl::I_VcsiUserNotificationApi& notApi,
        LmCdl::I_VectorDataDrawingApi& drawApi,
        LmCdl::I_MissionDrawingApi& missionApi,
        LmCdl::I_RouteApi& routeApi,
        LmCdl::I_TrackDrawingApi& trackApi,
        LmCdl::I_VideoStreamApiCollection& videoCollectionApi,
        LmCdl::I_GroundElevationApi& elevationApi);

    virtual ~MissionPlanningContentCreator();

private:
    Q_DISABLE_COPY(MissionPlanningContentCreator);

    void getPoiProperties(const LmCdl::ContextMenuEvent& event);

    void MissionPlanningContentCreator::startLoop();

    void initConextMenuItems();

    void getFlightPath();

    void runMission();

    void cancelMission();

    void connectToApiSignals();

    void drawMissionArea();

    void updatePois();

    void executeMissionAction();

    void changeUI(State newState);

    void updateUIState(State newState);

    void notify(const std::string& msg, Severity severity = Severity::Message);

    void draw(QList<MissionPlanningPolygon*> polygons, QList<MissionPlanningLine*> lines);

    void drawFlightPath();

    void clearMissionArea();

    void clearFlightPath();

    void notifyPeriodically();

    LmCdl::I_ContextMenuItem& missionBoundMenuItem_;
    LmCdl::I_ContextMenuItem& submitMissionMenuItem_;

    std::vector<std::vector<QGeoCoordinate>> pois_;
    LmCdl::I_PointOfInterestApi& poiApi_;
    LmCdl::I_VcsiUserNotificationApi& notApi_;
    LmCdl::I_VectorDataDrawingApi& drawApi_;
    LmCdl::I_MissionDrawingApi& missionApi_;
    LmCdl::I_RouteApi& routeApi_;
    LmCdl::I_TrackDrawingApi& trackApi_;
    LmCdl::I_VideoStreamApiCollection& videoCollectionApi_;
    LmCdl::I_GroundElevationApi& elevationApi_;

    LmCdl::I_VideoStreamApi* liveDroneFeed_;

    ImageProcessor imageProcessor_ = ImageProcessor();

    LmCdl::I_UserNotification* notification_;
    MissionPlanningDrawing* drawing_ = new MissionPlanningDrawing();
    BoundingBox missionBounds_;

    FlightPather flightPather_ = FlightPather();

    MissionDomain mission_;

    int m_state;
    volatile static double latitude;  // WGS84
    volatile static double longitude;  // WGS84
    volatile static double altitude;  // relative altitude, m
    volatile static double heading;  // degrees, 0 to 360
    volatile static double speed;  // meters per second
    volatile static double yaw;  // degrees, 0 to 360
    volatile static double battery;  // percentage, 0 to 1

    Drone* drone_;

    QTimer* timer_;
};