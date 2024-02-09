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
#include <LmCdl/I_Billboard.h>
#include <LmCdl/I_PlannedRouteCollection.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
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

volatile double MissionPlanningContentCreator::latitude = 0.0f;
volatile double MissionPlanningContentCreator::longitude = 0.0f;
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
    LmCdl::I_TrackDrawingApi& trackApi)
    : missionBoundMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , submitMissionMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , poiApi_(poiApi)
    , notApi_(notApi)
    , drawApi_(drawApi)
    , missionApi_(missionApi)
    , routeApi_(routeApi)
    , trackApi_(trackApi)
    , mapApi_(mapApi)
    , notification_(nullptr)
    , m_state(STARTUP)
    , mission_()
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

    connectToApiSignals();

    drone_ = new Drone(std::ref(latitude),
                       std::ref(longitude),
                       std::ref(altitude),
                       std::ref(heading),
                       std::ref(speed),
                       std::ref(yaw),
                       std::ref(battery),
                       mapApi_);

    trackApi.addDrawingForTrack(*drone_);

    updatePois();
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() {};

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

    if (flightPather_.canFly(missionBounds_)) {
        notify("Building Flight Path.");
        mission_.setPath(flightPather_.path());
        drawFlightPath();
        updateUIState(State::CanRunMission);
    } else {
        notify("Area is too large.");
    }
}

// " Connection URL format should be :"
// " For TCP : tcp://[server_host][:server_port]"
// " For UDP : udp://[bind_host][:bind_port]"
// " For Serial : serial:///path/to/serial/dev[:baudrate]"
// " For example, to connect to the simulator use URL: udp://:14540";

void MissionPlanningContentCreator::runMission()
{
    notify("Starting Mission.");
    updateUIState(State::CanCancelMission);

    std::vector<std::pair<float, float>> mavWaypoints;
    foreach(MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        mavWaypoints.emplace_back(waypoint->location().longitude(),
                                  waypoint->location().latitude());
    }

    drone_->setVisible(true);

    QFuture<void> future = QtConcurrent::run(sardinos::executeMissionVTOL,
                                             mavWaypoints,
                                             std::ref(latitude),
                                             std::ref(longitude),
                                             std::ref(altitude),
                                             std::ref(heading));
}

void MissionPlanningContentCreator::cancelMission()
{
    notify("Cancelling Mission.");

    updateUIState(State::CanGetFlightPath);

    updatePois();
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
    MissionPlanningContentCreator::State newState)
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
    };
}

void MissionPlanningContentCreator::updateUIState(State newState)
{
    if (newState != m_state) {
        m_state = newState;
        changeUI(newState);
    }
}
void MissionPlanningContentCreator::notify(const std::string& msg)
{
    notApi_.addNotification(new QLabel(QString(msg.c_str())));
}

void MissionPlanningContentCreator::notifyPeriodically()
{
    // (lat, long, alt) [heading]
    std::string msg = "(" + std::to_string(latitude) + ", "
        + std::to_string(longitude) + ", " + std::to_string(altitude) + ") ["
        + std::to_string(heading) + "]";

    notApi_.addNotification(new QLabel(QString(msg.c_str())));
}
