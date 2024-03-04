#pragma once

#include <QFuture>
#include <QList>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>
#include <vector>

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
#include <qgeocoordinate.h>

#include <Drone/Drone.h>
#include <Drone/DroneTelemetry.h>
#include <Drone/MissionManager.h>
#include <Image/ImageProcessor.h>
#include <UI/Drawing.h>
#include <UI/Line.h>
#include <UI/UIHandler.h>
#include <Waypoint/MissionDomain.h>
#include <Waypoint/Waypoint.h>
#include <Waypoint/WaypointConnector.h>
#include <Helpers/BoundingBox.h>
#include <UI/Notifications.h>

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
    ContentCreator(LmCdl::I_VcsiMapExtensionApi& mapApi,
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

    void updateDroneWidget();

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

    std::unique_ptr<DroneTelemetry> droneTelemetry = std::make_unique<DroneTelemetry>();

    ImageProcessor imageProcessor_;

    Drawing* drawing_ = new Drawing();

    BoundingBox missionBounds_;

    MissionDomain mission_;

    UIHandler uiHandler_;

    Notifications notis_;

    UIHandler::State m_state;

    QList<Target> targets_;

    Drone* drone_;

    MissionManager* missionManager_ {};

    QFuture<void> mavFut_;
    QFuture<void> cancelFut_;
};