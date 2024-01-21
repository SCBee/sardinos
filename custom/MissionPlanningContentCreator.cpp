#include <QCoreApplication>
#include <QGeoRectangle>
#include <QLabel>
#include <QTime>
#include <cmath>

#include <LmCdl/ContextMenuEvent.h>
#include <LmCdl/I_Billboard.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <LmCdl/VectorDataPolygonDrawing.h>
#include <MathExt.cpp>
#include <MissionPlanningContentCreator.h>
#include <qicon.h>
#include <MissionDomain.h>

MissionPlanningContentCreator::MissionPlanningContentCreator(
    LmCdl::I_VcsiMapExtensionApi& mapApi,
    LmCdl::I_PointOfInterestApi& poiApi,
    LmCdl::I_VcsiUserNotificationApi& notApi,
    LmCdl::I_VectorDataDrawingApi& drawApi,
    LmCdl::I_MissionDrawingApi& missionApi)
    : missionBoundMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , submitMissionMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , poiApi_(poiApi)
    , notApi_(notApi)
    , drawApi_(drawApi)
    , missionApi_(missionApi)
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
        try {
            mission_.setPath(flightPather_.path());
        } catch (const std::exception& e) {
            notify(e.what());
        }
        drawFlightPath();
        updateUIState(State::CanRunMission);
    } else {
        notify("Area is too large.");
    }
}

void MissionPlanningContentCreator::runMission()
{
    notify("Starting Mission.");

    updateUIState(State::CanCancelMission);
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
    else updateUIState(State::CannotGetFlightPath);

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
    foreach (LmCdl::I_SimpleWaypointConnector* waypointConnector, mission_.waypointConnectors()) {
        missionApi_.removeDrawingForWaypointConnector(*waypointConnector);
    }
    foreach (MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        missionApi_.removeDrawingForWaypoint(*waypoint);
    }

}

void MissionPlanningContentCreator::drawFlightPath()
{
    foreach (MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        missionApi_.addDrawingForWaypoint(*waypoint);
    }
    foreach (LmCdl::I_SimpleWaypointConnector* waypointConnector, mission_.waypointConnectors()) {
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
            submitMissionMenuItem_.setBackgroundColor(QColor(50, 100, 235, 180));
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
            submitMissionMenuItem_.setBackgroundColor(QColor(50, 100, 235, 180));
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
