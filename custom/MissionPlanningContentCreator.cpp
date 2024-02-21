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
#include <MathExt.h>
#include <MissionDomain.h>
#include <MissionPlanningContentCreator.h>
#include <QtConcurrent/QtConcurrent>
#include <SardinosPublisher.h>
#include <qicon.h>

using std::chrono::seconds;
using std::this_thread::sleep_for;

volatile double MissionPlanningContentCreator::latitude    = 51.0f;
volatile double MissionPlanningContentCreator::longitude   = -114.0f;
volatile double MissionPlanningContentCreator::altitude    = 0.0f;
volatile double MissionPlanningContentCreator::altitudeAbs = 0.0f;
volatile double MissionPlanningContentCreator::heading     = 0.0f;
volatile double MissionPlanningContentCreator::speed       = 0.0f;
volatile double MissionPlanningContentCreator::yaw         = 0.0f;
volatile double MissionPlanningContentCreator::battery     = 0.0f;

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
    , m_state(UIHandler::State::STARTUP)
    , mission_()
    , drone_(new Drone(mapApi))
    , timer_(new QTimer())
    , liveDroneFeed_(nullptr)
{
    init();
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() = default;

void MissionPlanningContentCreator::init()
{
    uiHandler_.initContextMenuItems(missionBoundMenuItem_, submitMissionMenuItem_);
    connectToApiSignals();
    startLoop();
    trackApi_.addDrawingForTrack(*drone_);
    updatePois();
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

void MissionPlanningContentCreator::getPoiProperties(
    const LmCdl::ContextMenuEvent& event)
{
    const auto& location = event.worldLocation();

    auto properties = LmCdl::VcsiPointOfInterestProperties(
        QString("Mission Bound"), location);

    poiApi_.addPointOfInterest(properties,
                               [this](const LmCdl::VcsiPointOfInterestId&)
                               { updatePois(); });

    uiHandler_.updateUIState(UIHandler::State::CanGetFlightPath, m_state, missionBoundMenuItem_, submitMissionMenuItem_, mission_, missionApi_, drawApi_);
}

void MissionPlanningContentCreator::getFlightPath()
{
    updatePois();

    notis_.notify("Building Flight Path.", notApi_);

    mission_.setPath(sardinos::FlightPather::getPath(missionBounds_));

    drawing_->drawFlightPath(mission_, missionApi_);

    uiHandler_.updateUIState(UIHandler::State::CanRunMission, m_state, missionBoundMenuItem_, submitMissionMenuItem_, mission_, missionApi_, drawApi_);
}

void MissionPlanningContentCreator::runMission()
{
    if (!sardinos::FlightPather::canFly(mission_.waypoints())) {
        notis_.notify("Flight path is too long.", notApi_, Notifications::Severity::Warning);
        return;
    }

    notis_.notify("Starting Mission.", notApi_, Notifications::Severity::Continue);

    uiHandler_.updateUIState(UIHandler::State::CanCancelMission, m_state, missionBoundMenuItem_, submitMissionMenuItem_, mission_, missionApi_, drawApi_);

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
    auto pipeline =
        "udpsrc port=5200 caps = \"application/x-rtp, media=(string)video, "
        "clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" "
        "! rtph264depay ! decodebin ! videoconvert ! appsink";

    liveDroneFeed_ =
        &videoCollectionApi_.registerStream(uri, "Live drone stream");

    auto imageProcessor = new ImageProcessor();
    
    QFuture<void> testImageProcFuture = QtConcurrent::run([this, imageProcessor, uri, pipeline]() { imageProcessor->init(uri); });

    QFuture<void> future = QtConcurrent::run(
        [mavWaypoints]()
        {
            sardinos::executeMissionVTOL(std::ref(mavWaypoints),
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
    notis_.notify("Cancelling Mission.", notApi_, Notifications::Severity::Warning);

    videoCollectionApi_.unregisterStream(*liveDroneFeed_);

    updatePois();

    uiHandler_.updateUIState(UIHandler::State::CanGetFlightPath, m_state, missionBoundMenuItem_, submitMissionMenuItem_, mission_, missionApi_, drawApi_);
}

void MissionPlanningContentCreator::updatePois()
{
    sardinos::MathExt::delay(20);

    auto points = poiApi_.pointsOfInterest();

    missionBounds_ = sardinos::MathExt::findSmallestBoundingBox(points);

    pois_.clear();

    for (const auto& p : points)
        pois_.push_back({p.pointOfInterest().location()});

    if (pois_.size() > 2)
        uiHandler_.updateUIState(UIHandler::State::CanGetFlightPath, m_state, missionBoundMenuItem_, submitMissionMenuItem_, mission_, missionApi_, drawApi_);
    else
        uiHandler_.updateUIState(UIHandler::State::CannotGetFlightPath, m_state, missionBoundMenuItem_, submitMissionMenuItem_, mission_, missionApi_, drawApi_);

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
