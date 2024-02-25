#include <QCoreApplication>
#include <QFuture>
#include <QGeoRectangle>
#include <QLabel>
#include <QTime>
#include <QTimer>
#include <thread>
#include <utility>

#include <Drone.h>
#include <LmCdl/ContextMenuEvent.h>
#include <LmCdl/GroundElevation.h>
#include <LmCdl/I_PlannedRouteCollection.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_VideoStreamApi.h>
#include <LmCdl/I_VideoWindowApi.h>
#include <LmCdl/StanagRoute.h>
#include <LmCdl/StanagWaypoint.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <LmCdl/VectorDataPolygonDrawing.h>
#include <MissionDomain.h>
#include <MissionPlanningContentCreator.h>
#include <QtConcurrent/QtConcurrent>
#include <qicon.h>

using std::chrono::seconds;
using std::this_thread::sleep_for;

std::mutex mutex;

volatile double MissionPlanningContentCreator::latitude    = 51.0f;
volatile double MissionPlanningContentCreator::longitude   = -114.0f;
volatile double MissionPlanningContentCreator::altitude    = 0.0f;
volatile double MissionPlanningContentCreator::altitudeAbs = 0.0f;
volatile double MissionPlanningContentCreator::heading     = 0.0f;
volatile double MissionPlanningContentCreator::speed       = 0.0f;
volatile double MissionPlanningContentCreator::yaw         = 0.0f;
volatile double MissionPlanningContentCreator::battery     = 0.0f;

volatile bool MissionPlanningContentCreator::connectedToDrone_ = false;

MissionPlanningContentCreator::MissionPlanningContentCreator(
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
    , imageProcessor_(std::ref(targets_), std::ref(latitude), std::ref(longitude))
    , timer_(new QTimer())
{
    init();
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() = default;

void MissionPlanningContentCreator::init()
{
    uiHandler_.initContextMenuItems(missionBoundMenuItem_,
                                    submitMissionMenuItem_);
    connectToApiSignals();
    startLoop();
    trackApi_.addDrawingForTrack(*drone_);
    updatePois();

    std::string connectStr = "udp://:14550";
    // auto connectStr = "serial://COM3:57600";

    QtConcurrent::run(
        [this, connectStr] {
            missionManager_ = new MissionManager(connectStr, std::ref(connectedToDrone_));
        });

}

void MissionPlanningContentCreator::startLoop()
{
    timer_->setInterval(1000);

    connect(
        timer_,
        &QTimer::timeout,
        this,
        [this]()
        {
            this->drone_->updateValues(
                latitude, longitude, altitude, heading, speed, yaw, battery);

            this->showTargets();

            this->checkConnection();
        });

    timer_->start();
}

void MissionPlanningContentCreator::connectToApiSignals()
{
    connect(&missionBoundMenuItem_,
            &LmCdl::I_ContextMenuItem::clicked,
            this,
            &MissionPlanningContentCreator::getPoiProperties);

    connect(&submitMissionMenuItem_,
            &LmCdl::I_ContextMenuItem::clicked,
            this,
            &MissionPlanningContentCreator::executeMissionAction);

    connect(&poiApi_,
            &LmCdl::I_PointOfInterestApi::pointOfInterestRemoved,
            this,
            &MissionPlanningContentCreator::updatePois);
}

void MissionPlanningContentCreator::showTargets()
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

void MissionPlanningContentCreator::checkConnection()
{
    if (alreadyConnected_)
        return;
        
    if (connectedToDrone_) {
        notis_.notify("Successfully connect to drone", notApi_, Notifications::Continue);
        alreadyConnected_ = true;
    }
}

void MissionPlanningContentCreator::getPoiProperties(
    const LmCdl::ContextMenuEvent& event)
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

void MissionPlanningContentCreator::getFlightPath()
{
    updatePois();

    notis_.notify("Building Flight Path.", notApi_);

    mission_.setPath(sardinos::getPath(missionBounds_));

    drawing_->drawFlightPath(mission_, missionApi_);

    uiHandler_.updateUIState(UIHandler::State::CanRunMission,
                             m_state,
                             missionBoundMenuItem_,
                             submitMissionMenuItem_,
                             mission_,
                             missionApi_,
                             drawApi_);
}

void MissionPlanningContentCreator::runMission()
{
    if (!sardinos::canFly(mission_.waypoints())) {
        notis_.notify("Flight path is too long.",
                      notApi_,
                      Notifications::Severity::Warning);
        return;
    }

    if (!connectedToDrone_) {
        notis_.notify("Not connected to a drone.",
                      notApi_,
                      Notifications::Severity::Danger);
        return;
    }

    cancelFut_.cancel();

    notis_.notify("Starting Mission.", notApi_, Notifications::Continue);

    uiHandler_.updateUIState(UIHandler::State::CanCancelMission,
                             m_state,
                             missionBoundMenuItem_,
                             submitMissionMenuItem_,
                             mission_,
                             missionApi_,
                             drawApi_);

    std::vector<std::pair<float, float>> mavWaypoints;

    foreach(MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        mavWaypoints.emplace_back(waypoint->location().longitude(),
                                  waypoint->location().latitude());
    }

    drone_->setVisible(true);

    mission_.startMission();

    // Sample video
    auto uri = "C:/Program Files/LMCDL/vcsi/sdk/examples/custom/SampleFeed.mp4";

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
                                                std::ref(latitude),
                                                std::ref(longitude),
                                                std::ref(altitude),
                                                std::ref(altitudeAbs),
                                                std::ref(heading),
                                                std::ref(speed),
                                                std::ref(yaw),
                                                std::ref(battery));
        });
}

void MissionPlanningContentCreator::cancelMission()
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

void MissionPlanningContentCreator::updatePois()
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

void MissionPlanningContentCreator::executeMissionAction()
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
