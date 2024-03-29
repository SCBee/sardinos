#pragma once

#include <QFuture>
#include <QList>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>
#include <vector>

#include <BoundingBox.h>
#include <Drawing.h>
#include <Drone.h>
#include <ImageProcessor.h>
#include <Line.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_GroundElevationApi.h>
#include <LmCdl/I_MissionDrawingApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_RouteApi.h>
#include <LmCdl/I_TrackDrawingApi.h>
#include <LmCdl/I_TrackVisualization.h>
#include <LmCdl/I_UserNotification.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_VideoStreamApiCollection.h>
#include <LmCdl/UniqueIdentifier.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiMilStdCode.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <MissionDomain.h>
#include <MissionManager.h>
#include <Notifications.h>
#include <Polygon.h>
#include <UIHandler.h>
#include <Waypoint.h>
#include <WaypointConnector.h>
#include <qgeocoordinate.h>

namespace LmCdl
{
    class I_VcsiMapExtensionApi;

    class I_PointOfInterestApi;

    class I_VcsiApplicationApi;
}  // namespace LmCdl

class ContentCreator : public QObject
{
    Q_OBJECT

public:
    ContentCreator(
        LmCdl::I_VcsiMapExtensionApi& mapApi,
        LmCdl::I_PointOfInterestApi& poiApi,
        LmCdl::I_VcsiUserNotificationApi& notApi,
        LmCdl::I_VectorDataDrawingApi& drawApi,
        LmCdl::I_MissionDrawingApi& missionApi,
        LmCdl::I_RouteApi& routeApi,
        LmCdl::I_TrackDrawingApi& trackApi,
        LmCdl::I_VideoStreamApiCollection& videoCollectionApi);

    ~ContentCreator() override;

private:
    Q_DISABLE_COPY(ContentCreator);

    void getPoiProperties(const LmCdl::ContextMenuEvent& event);

    void startLoop();

    void init();

    void getFlightPath();

    void runMission();

    void cancelMission();

    void connectToApiSignals();

    void updatePois();

    void executeMissionAction();

    void showTargets();

    void checkConnection();

    void forceLand();

    LmCdl::I_ContextMenuItem& missionBoundMenuItem_;
    LmCdl::I_ContextMenuItem& submitMissionMenuItem_;
    LmCdl::I_ContextMenuItem& forceLandMissionMenuItem_;

    std::vector<std::vector<QGeoCoordinate>> pois_;

    LmCdl::I_PointOfInterestApi& poiApi_;
    LmCdl::I_VcsiUserNotificationApi& notApi_;
    LmCdl::I_VectorDataDrawingApi& drawApi_;
    LmCdl::I_MissionDrawingApi& missionApi_;
    [[maybe_unused]] LmCdl::I_RouteApi& routeApi_;
    LmCdl::I_TrackDrawingApi& trackApi_;
    LmCdl::I_VideoStreamApiCollection& videoCollectionApi_;
    LmCdl::I_VcsiMapExtensionApi& mapApi_;

    ImageProcessor imageProcessor_;

    Drawing* drawing_ = new Drawing();
    BoundingBox missionBounds_;

    MissionDomain mission_;

    UIHandler uiHandler_;

    Notifications notis_;

    UIHandler::State m_state;

    volatile static double latitude;  // WGS84
    volatile static double longitude;  // WGS84
    volatile static double altitude;  // relative altitude, m
    volatile static double altitudeAbs;  // absolute altitude, m
    volatile static double heading;  // degrees, 0 to 360
    volatile static double speed;  // meters per second
    volatile static double yaw;  // degrees, 0 to 360
    volatile static double battery;  // percentage, 0 to 1

    volatile static bool connectedToDrone_;
    bool alreadyConnected_ = false;

    QList<Target> targets_;

    Drone* drone_;

    QTimer* timer_;

    MissionManager* missionManager_ {};

    QFuture<void> mavFut_;
    QFuture<void> cancelFut_;
};