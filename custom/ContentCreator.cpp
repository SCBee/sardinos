#include <QGeoRectangle>
#include <thread>
#include <utility>

#include <ContentCreator.h>
#include <LmCdl/ContextMenuEvent.h>
#include <LmCdl/I_PlannedRouteCollection.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_VideoStreamApi.h>
#include <LmCdl/I_VideoWindowApi.h>
#include <LmCdl/StanagRoute.h>
#include <LmCdl/StanagWaypoint.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <QtConcurrent/QtConcurrent>

#include "Waypoint/MissionDomain.h"

using std::chrono::seconds;
using std::this_thread::sleep_for;

std::mutex mutex;

ContentCreator::ContentCreator(
    LmCdl::I_VcsiMapExtensionApi& mapApi,
    LmCdl::I_PointOfInterestApi& poiApi,
    LmCdl::I_VcsiUserNotificationApi& notApi,
    LmCdl::I_VectorDataDrawingApi& drawApi,
    LmCdl::I_MissionDrawingApi& missionApi,
    LmCdl::I_RouteApi& routeApi,
    LmCdl::I_TrackDrawingApi& trackApi,
    LmCdl::I_VideoStreamApiCollection& videoCollectionApi)
    : missionBoundMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , submitMissionMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , forceLandMissionMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , poiApi_(poiApi)
    , notApi_(notApi)
    , drawApi_(drawApi)
    , missionApi_(missionApi)
    , routeApi_(routeApi)
    , trackApi_(trackApi)
    , videoCollectionApi_(videoCollectionApi)
    , mapApi_(mapApi)
    , m_state(UIHandler::State::STARTUP)
    , mission_()
    , drone_(new Drone(mapApi))
    , imageProcessor_(std::ref(targets_), droneTelemetry.get())
{
    init();

    droneTelemetry->setLatitude(51.37652);
    droneTelemetry->setLongitude(-114.2974);
    droneTelemetry->setAltitude(2000);
}

ContentCreator::~ContentCreator() = default;

void ContentCreator::init()
{
    uiHandler_.initContextMenuItems(missionBoundMenuItem_,
                                    submitMissionMenuItem_,
                                    forceLandMissionMenuItem_);
    connectToApiSignals();
    trackApi_.addDrawingForTrack(*drone_);
    updatePois();

    std::string connectStr = "udp://:14550";
    // auto connectStr = "serial://COM3:57600";

    QtConcurrent::run(
        [this, connectStr]
        { missionManager_ = new MissionManager(connectStr, droneTelemetry.get()); });
}

void ContentCreator::updateDroneWidget()
{
    drone_->updateValues(droneTelemetry->latitude(),
                         droneTelemetry->longitude(),
                         droneTelemetry->altitude(),
                         droneTelemetry->heading(),
                         droneTelemetry->speed(),
                         droneTelemetry->yaw(),
                         droneTelemetry->battery());
    showTargets();
}

void ContentCreator::connectToApiSignals()
{
    connect(&missionBoundMenuItem_,
            &LmCdl::I_ContextMenuItem::clicked,
            this,
            &ContentCreator::getPoiProperties);

    connect(&submitMissionMenuItem_,
            &LmCdl::I_ContextMenuItem::clicked,
            this,
            &ContentCreator::executeMissionAction);

    connect(&forceLandMissionMenuItem_,
            &LmCdl::I_ContextMenuItem::clicked,
            this,
            &ContentCreator::forceLand);

    connect(&poiApi_,
            &LmCdl::I_PointOfInterestApi::pointOfInterestRemoved,
            this,
            &ContentCreator::updatePois);

    connect(droneTelemetry.get(),
            &DroneTelemetry::telemetryUpdated,
            this,
            &ContentCreator::updateDroneWidget,
            Qt::QueuedConnection);

    connect(droneTelemetry.get(),
            &DroneTelemetry::connectionStatusChanged,
            this,
            &ContentCreator::checkConnection,
            Qt::QueuedConnection);

    connect(droneTelemetry.get(),
            &DroneTelemetry::targetFound,
            this,
            &ContentCreator::showTargets,
            Qt::QueuedConnection);
}

void ContentCreator::forceLand()
{
    if (!droneTelemetry->isConnected()) {
        notis_.notify("Not connected to a drone.",
                      notApi_,
                      Notifications::Severity::Danger);
        return;
    }

    QtConcurrent::run([this] { missionManager_->returnHome(); });
}

void ContentCreator::showTargets()
{
    mutex.lock();

    auto targets = targets_;

    targets_.clear();

    mutex.unlock();

    for (Target& target : targets) {
        auto loc = target.Location;

        auto lat = loc.latitude();
        auto lon = loc.longitude();

        auto targetWidget =
            new TargetWidget(loc.latitude(), loc.longitude(), target.Mat);

        auto widget = &mapApi_.addGraphicsWidget(targetWidget);

        widget->setLocation(loc);
        widget->setVisible(true);

        notis_.notify("Target Found at: " + std::to_string(lat) + ", "
                          + std::to_string(lon),
                      notApi_,
                      Notifications::Continue);
    }
}

void ContentCreator::checkConnection()
{
    if (droneTelemetry->isConnected()) {
        notis_.notify("Successfully connected to drone",
                      notApi_,
                      Notifications::Continue);
    }
}

void ContentCreator::getPoiProperties(const LmCdl::ContextMenuEvent& event)
{
    const auto& location = event.worldLocation();

    auto properties = LmCdl::VcsiPointOfInterestProperties(
        QString("Mission Bound"), location);

    poiApi_.addPointOfInterest(properties,
                               [this](const LmCdl::VcsiPointOfInterestId&)
                               { updatePois(); });

    uiHandler_.updateUIState(UIHandler::State::CanGetFlightPath,
                             m_state,
                             missionBoundMenuItem_,
                             submitMissionMenuItem_,
                             mission_,
                             missionApi_,
                             drawApi_);
}

void ContentCreator::getFlightPath()
{
    updatePois();

    notis_.notify("Building Flight Path.", notApi_);

    mission_.setPath(sardinos::getCheesePath(missionBounds_));

    drawing_->drawFlightPath(mission_, missionApi_);

    uiHandler_.updateUIState(UIHandler::State::CanRunMission,
                             m_state,
                             missionBoundMenuItem_,
                             submitMissionMenuItem_,
                             mission_,
                             missionApi_,
                             drawApi_);
}

void ContentCreator::runMission()
{
    if (!sardinos::canFly(mission_.waypoints())) {
        notis_.notify("Flight path is too long.",
                      notApi_,
                      Notifications::Severity::Warning);
        return;
    }

    if (!droneTelemetry->isConnected()) {
        notis_.notify("Not connected to a drone.",
                      notApi_,
                      Notifications::Severity::Danger);
        return;
    }

    cancelFut_.cancel();

    notis_.notify("Starting Mission.", notApi_, Notifications::Continue, 15000);

    uiHandler_.updateUIState(UIHandler::State::CanCancelMission,
                             m_state,
                             missionBoundMenuItem_,
                             submitMissionMenuItem_,
                             mission_,
                             missionApi_,
                             drawApi_);

    std::vector<std::pair<float, float>> mavWaypoints;

    foreach(Waypoint* waypoint, mission_.waypoints()) {
        mavWaypoints.emplace_back(waypoint->location().longitude(),
                                  waypoint->location().latitude());
    }

    drone_->setVisible(true);

    mission_.startMission();

    // Sample video
    auto uri = "C:/Program Files/LMCDL/vcsi/sdk/examples/custom/Assets/DroneFlight1.mp4";

    // GStreamer pipeline string adapted for OpenCV
    // auto uri =
    //     "udpsrc port=5200 caps = \"application/x-rtp, media=(string)video, "
    //     "clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\"
    //     "
    //     "! rtph264depay ! decodebin ! videoconvert ! appsink";

    QtConcurrent::run([this, uri] { imageProcessor_.init(uri); });

    mavFut_ = QtConcurrent::run(
        [this, mavWaypoints]()
        {
            missionManager_->executeMissionQuad(std::ref(mavWaypoints),
                                                droneTelemetry.get());
        });
}

void ContentCreator::cancelMission()
{
    notis_.notify(
        "Cancelling Mission.", notApi_, Notifications::Severity::Warning);

    mavFut_.cancel();

    cancelFut_ = QtConcurrent::run([this] { missionManager_->returnHome(); });

    updatePois();

    imageProcessor_.stop();

    uiHandler_.updateUIState(UIHandler::State::CanGetFlightPath,
                             m_state,
                             missionBoundMenuItem_,
                             submitMissionMenuItem_,
                             mission_,
                             missionApi_,
                             drawApi_);
}

void ContentCreator::updatePois()
{
    sardinos::delay(20);

    auto points = poiApi_.pointsOfInterest();

    missionBounds_ = sardinos::findSmallestBoundingBox(points);

    pois_.clear();

    for (const auto& p : points)
        pois_.push_back({p.pointOfInterest().location()});

    if (pois_.size() > 2)
        uiHandler_.updateUIState(UIHandler::State::CanGetFlightPath,
                                 m_state,
                                 missionBoundMenuItem_,
                                 submitMissionMenuItem_,
                                 mission_,
                                 missionApi_,
                                 drawApi_);
    else
        uiHandler_.updateUIState(UIHandler::State::CannotGetFlightPath,
                                 m_state,
                                 missionBoundMenuItem_,
                                 submitMissionMenuItem_,
                                 mission_,
                                 missionApi_,
                                 drawApi_);

    drawing_->drawMissionArea(pois_, missionBounds_, drawApi_);
}

void ContentCreator::executeMissionAction()
{
    switch (m_state) {
        case UIHandler::State::CanGetFlightPath:
            getFlightPath();
            break;
        case UIHandler::State::CanRunMission:
            runMission();
            break;
        case UIHandler::State::CanCancelMission:
            cancelMission();
            break;
        default:
            break;
    }
}
