#include <QCoreApplication>
#include <QFuture>
#include <QGeoRectangle>
#include <QLabel>
#include <QTime>
#include <QTimer>
#include <cmath>
#include <iostream>
#include <thread>

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
#include <MathExt.cpp>
#include <MissionDomain.h>
#include <MissionPlanningContentCreator.h>
#include <QtConcurrent/QtConcurrent>
#include <SardinosPublisher.h>
#include <qicon.h>

using std::chrono::seconds;
using std::this_thread::sleep_for;

volatile double MissionPlanningContentCreator::latitude = 51.0f;
volatile double MissionPlanningContentCreator::longitude = -114.0f;
volatile double MissionPlanningContentCreator::altitude = 0.0f;
volatile double MissionPlanningContentCreator::heading = 0.0f;
volatile double MissionPlanningContentCreator::speed = 0.0f;
volatile double MissionPlanningContentCreator::yaw = 0.0f;
volatile double MissionPlanningContentCreator::battery = 0.0f;

MissionPlanningContentCreator::MissionPlanningContentCreator(
    LmCdl::I_VcsiMapExtensionApi& mapApi,
    LmCdl::I_PointOfInterestApi& poiApi,
    LmCdl::I_VcsiUserNotificationApi& notApi,
    LmCdl::I_VectorDataDrawingApi& drawApi,
    LmCdl::I_MissionDrawingApi& missionApi,
    LmCdl::I_RouteApi& routeApi,
    LmCdl::I_TrackDrawingApi& trackApi,
    LmCdl::I_VideoStreamApiCollection& videoCollectionApi,
    LmCdl::I_GroundElevationApi& elevationApi)
    : missionBoundMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , submitMissionMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , poiApi_(poiApi)
    , notApi_(notApi)
    , drawApi_(drawApi)
    , missionApi_(missionApi)
    , routeApi_(routeApi)
    , trackApi_(trackApi)
    , videoCollectionApi_(videoCollectionApi)
    , elevationApi_(elevationApi)
    , notification_(nullptr)
    , m_state(STARTUP)
    , mission_()
    , drone_(new Drone(mapApi))
    , timer_(new QTimer())
{
    initContextMenuItems();

    connectToApiSignals();

    startLoop();

    trackApi.addDrawingForTrack(*drone_);

    updatePois();
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() {};

void MissionPlanningContentCreator::initContextMenuItems()
{
    missionBoundMenuItem_.setBackgroundColor(QColor(235, 12, 12, 180));
    missionBoundMenuItem_.setDescription("Add Mission Bound");
    missionBoundMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    missionBoundMenuItem_.setIcon(":/MissionPlanning/missionPlanningDinoIcon");
    missionBoundMenuItem_.setVisible(true);

    submitMissionMenuItem_.setBackgroundColor(QColor(50, 100, 235, 180));
    submitMissionMenuItem_.setDescription("Get Flight Path");
    submitMissionMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Top);
    submitMissionMenuItem_.setIcon(":/MissionPlanning/UCIcon");
    submitMissionMenuItem_.setVisible(false);
}

void MissionPlanningContentCreator::startLoop()
{
    timer_->setInterval(1000);

    connect(
        timer_,
        &QTimer::timeout,
        this,
        [=]()
        {
            drone_->updateValues(
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

    updateUIState(State::CanGetFlightPath);
}

void MissionPlanningContentCreator::getFlightPath()
{
    updatePois();

    notify("Building Flight Path.");

    mission_.setPath(flightPather_.getPath(missionBounds_));

    drawFlightPath();

    updateUIState(State::CanRunMission);
}

void MissionPlanningContentCreator::runMission()
{
    if (!flightPather_.canFly(mission_.waypoints())) {
        notify("Flight path is too long.", Severity::Warning);
        return;
    }

    notify("Starting Mission.", Severity::Continue);

    updateUIState(State::CanCancelMission);

    std::vector<std::pair<float, float>> mavWaypoints;

    foreach(MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        mavWaypoints.emplace_back(waypoint->location().longitude(),
                                  waypoint->location().latitude());
    }

    drone_->setVisible(true);

    //    mission_.startMission();
    //
    //    auto uri =
    //        "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/"
    //        "BigBuckBunny.mp4";
    //
    //    liveDroneFeed_ =
    //        &videoCollectionApi_.registerStream(uri, "Live drone stream");
    //
    //    imageProcessor_.init(uri);

    QFuture<void> future = QtConcurrent::run(sardinos::executeMissionVTOL,
                                             mavWaypoints,
                                             std::ref(latitude),
                                             std::ref(longitude),
                                             std::ref(altitude),
                                             std::ref(heading));
}

void MissionPlanningContentCreator::cancelMission()
{
    notify("Cancelling Mission.", Severity::Warning);

    videoCollectionApi_.unregisterStream(*liveDroneFeed_);

    updatePois();

    updateUIState(State::CanGetFlightPath);
}

void MissionPlanningContentCreator::updatePois()
{
    MathExt().delay(20);

    auto points = poiApi_.pointsOfInterest();

    missionBounds_ = MathExt().findSmallestBoundingBox(points);

    pois_.clear();

    for (const auto& p : points)
        pois_.push_back({p.pointOfInterest().location()});

    if (pois_.size() > 2)
        updateUIState(State::CanGetFlightPath);
    else
        updateUIState(State::CannotGetFlightPath);

    drawMissionArea();
}

Q_SLOT void MissionPlanningContentCreator::drawMissionArea()
{
    auto polygon = QGeoPolygon(missionBounds_.list());

    auto polygons = QList<MissionPlanningPolygon*>();

    polygons.append(new MissionPlanningPolygon(polygon));

    auto lines = QList<MissionPlanningLine*>();

    MathExt().cvhull(pois_);

    for (auto i = 1; i < pois_.size(); i++) {
        lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[i - 1][0]));
        if (i == pois_.size() - 1)
            lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[0][0]));
    }

    draw(polygons, lines);
}

void MissionPlanningContentCreator::draw(
    QList<MissionPlanningPolygon*> polygons, QList<MissionPlanningLine*> lines)
{
    drawing_->clear();
    drawing_->addPolygons(polygons);
    drawing_->addLines(lines);
    drawing_->update();

    drawApi_.removeDrawingForVectorData(*drawing_);
    drawApi_.addDrawingForVectorData(
        *drawing_,
        LmCdl::I_VectorDataDrawingApi::DrawingMode::
            OptimizedForFrequentChanges);
}

void MissionPlanningContentCreator::clearMissionArea()
{
    draw(QList<MissionPlanningPolygon*>(), QList<MissionPlanningLine*>());
}

void MissionPlanningContentCreator::clearFlightPath()
{
    foreach(LmCdl::I_SimpleWaypointConnector* waypointConnector,
            mission_.waypointConnectors())
    {
        missionApi_.removeDrawingForWaypointConnector(*waypointConnector);
    }
    foreach(MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        missionApi_.removeDrawingForWaypoint(*waypoint);
    }
}

void MissionPlanningContentCreator::drawFlightPath()
{
    foreach(MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        missionApi_.addDrawingForWaypoint(*waypoint);
    }
    foreach(LmCdl::I_SimpleWaypointConnector* waypointConnector,
            mission_.waypointConnectors())
    {
        missionApi_.addDrawingForWaypointConnector(*waypointConnector);
    }
}

void MissionPlanningContentCreator::changeUI(
    const MissionPlanningContentCreator::State& newState)
{
    switch (newState) {
        case CanGetFlightPath:
            missionBoundMenuItem_.setVisible(true);
            submitMissionMenuItem_.setVisible(true);
            submitMissionMenuItem_.setBackgroundColor(
                QColor(50, 100, 235, 180));
            submitMissionMenuItem_.setDescription("Get Flight Path");
            clearFlightPath();
            break;
        case CannotGetFlightPath:
            missionBoundMenuItem_.setVisible(true);
            submitMissionMenuItem_.setVisible(false);
            clearFlightPath();
            break;
        case CanRunMission:
            missionBoundMenuItem_.setVisible(true);
            submitMissionMenuItem_.setVisible(true);
            submitMissionMenuItem_.setBackgroundColor(QColor(12, 235, 12, 180));
            submitMissionMenuItem_.setDescription("Begin Mission");
            clearMissionArea();
            break;
        case CanCancelMission:
            missionBoundMenuItem_.setVisible(false);
            submitMissionMenuItem_.setVisible(true);
            submitMissionMenuItem_.setBackgroundColor(QColor(235, 12, 12, 180));
            submitMissionMenuItem_.setDescription("Cancel Mission");
            clearMissionArea();
            break;
        default:
            missionBoundMenuItem_.setVisible(true);
            submitMissionMenuItem_.setVisible(true);
            submitMissionMenuItem_.setBackgroundColor(
                QColor(50, 100, 235, 180));
            submitMissionMenuItem_.setDescription("Get Flight Path");
            clearFlightPath();
            break;
    };
}

void MissionPlanningContentCreator::executeMissionAction()
{
    switch (m_state) {
        case CanGetFlightPath:
            getFlightPath();
            break;
        case CanRunMission:
            runMission();
            break;
        case CanCancelMission:
            cancelMission();
            break;
        default:
            break;
    };
}

void MissionPlanningContentCreator::updateUIState(const State& newState)
{
    if (newState != m_state) {
        m_state = newState;
        changeUI(m_state);
    }
}

void MissionPlanningContentCreator::notify(const std::string& msg,
                                           Severity severity)
{
    auto label = new QLabel(QString(msg.c_str()));

    label->setStyleSheet("color: black;");

    switch (severity) {
        case Message:
            notApi_.addNotification(label).setBackgroundColor(Qt::white);
            break;
        case Continue:
            notApi_.addNotification(label).setBackgroundColor(Qt::green);
            break;
        case Warning:
            notApi_.addNotification(label).setBackgroundColor(Qt::yellow);
            break;
        case Danger:
            notApi_.addNotification(label).setBackgroundColor(Qt::red);
            break;
    };
}

void MissionPlanningContentCreator::notifyPeriodically()
{
    // (lat, long, alt) [heading]
    std::string msg = "(" + std::to_string(latitude) + ", "
        + std::to_string(longitude) + ", " + std::to_string(altitude) + ") ["
        + std::to_string(heading) + "]";

    notApi_.addNotification(new QLabel(QString(msg.c_str())));
}
