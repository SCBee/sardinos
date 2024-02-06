#pragma once

#include <QList>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>
#include <vector>

#include <BoundingBox.h>
#include <FlightPather.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_MissionDrawingApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_RouteApi.h>
#include <LmCdl/I_UserNotification.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
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
#include <MissionplanningWaypointConnector.h>
#include <qgeocoordinate.h>

namespace LmCdl
{
class I_VcsiMapExtensionApi;

class I_PointOfInterestApi;

class I_VcsiApplicationApi;
}  // namespace LmCdl

const auto SCANWIDTHMETERS = 20;
const auto MAXDISTANCEMETERS = 2000;
const auto TURNINGRADIUSMETERS = 10;

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

    MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi& mapApi,
                                  LmCdl::I_PointOfInterestApi& poiApi,
                                  LmCdl::I_VcsiUserNotificationApi& notApi,
                                  LmCdl::I_VectorDataDrawingApi& drawApi,
                                  LmCdl::I_MissionDrawingApi& missionApi,
                                  LmCdl::I_RouteApi& routeApi);

    virtual ~MissionPlanningContentCreator();

private:
    Q_DISABLE_COPY(MissionPlanningContentCreator);

    void getPoiProperties(const LmCdl::ContextMenuEvent& event);

    void getFlightPath();

    void runMission();

    void cancelMission();

    void connectToApiSignals();

    void drawMissionArea();

    void updatePois();

    void executeMissionAction();

    void changeUI(State newState);

    void updateUIState(State newState);

    void notify(const std::string& msg);

    void draw(QList<MissionPlanningPolygon*> polygons,
              QList<MissionPlanningLine*> lines);

    void drawFlightPath();

    void clearMissionArea();

    void clearFlightPath();

    LmCdl::I_ContextMenuItem& missionBoundMenuItem_;
    LmCdl::I_ContextMenuItem& submitMissionMenuItem_;

    std::vector<std::vector<QGeoCoordinate>> pois_;
    LmCdl::I_PointOfInterestApi& poiApi_;
    LmCdl::I_VcsiUserNotificationApi& notApi_;
    LmCdl::I_VectorDataDrawingApi& drawApi_;
    LmCdl::I_MissionDrawingApi& missionApi_;
    LmCdl::I_RouteApi& routeApi_;

    LmCdl::I_UserNotification* notification_;
    MissionPlanningDrawing* drawing_ = new MissionPlanningDrawing();
    BoundingBox missionBounds_;

    FlightPather flightPather_ =
        FlightPather(TURNINGRADIUSMETERS, SCANWIDTHMETERS, MAXDISTANCEMETERS);

    int m_state;
};