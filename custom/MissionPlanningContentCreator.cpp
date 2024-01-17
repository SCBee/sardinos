#include <MissionPlanningContentCreator.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <qwidget.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <qicon.h>
#include <LmCdl/I_Billboard.h>
#include <LmCdl/ContextMenuEvent.h>
#include <QLabel>
#include <QTimer>
#include <string>
#include <sstream>
#include <iomanip>
#include <LmCdl/VectorDataPolygonDrawing.h>
#include <QGeoRectangle>
#include <LmCdl/VectorDataDrawing.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <QTime>
#include <QCoreApplication>
#include <cmath>
#include <MathExt.cpp>

MissionPlanningContentCreator::MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi &mapApi,
                                                             LmCdl::I_PointOfInterestApi &poiApi,
                                                             LmCdl::I_VcsiUserNotificationApi &notApi,
                                                             LmCdl::I_VectorDataDrawingApi &drawApi)
    : missionBoundMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , submitMissionMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , poiApi_(poiApi)
    , notApi_(notApi)
    , drawApi_(drawApi)
    , notification_(nullptr)
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

MissionPlanningContentCreator::~MissionPlanningContentCreator(){};

void MissionPlanningContentCreator::connectToApiSignals()
{
    connect(&missionBoundMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::getPoiProperties);

    connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::getFlightPath);

    connect(&poiApi_, &LmCdl::I_PointOfInterestApi::pointOfInterestRemoved, this, &MissionPlanningContentCreator::updatePois);
}

void MissionPlanningContentCreator::getPoiProperties(const LmCdl::ContextMenuEvent &event)
{
    const auto &location = event.worldLocation();

    auto properties = LmCdl::VcsiPointOfInterestProperties(QString("Mission Bound"), location);

    poiApi_.addPointOfInterest(properties, [this](const LmCdl::VcsiPointOfInterestId){ updatePois(); });

    setState(State::CanGetFlightPath);
}

void MissionPlanningContentCreator::getFlightPath()
{
    updatePois();

    auto lines = flightPather_.getFlightPath(missionBounds_);

    if (lines.isEmpty())
    {
        notApi_.addNotification(new QLabel(QString("Search area too big")));
    }
    else
    {
        draw(QList<MissionPlanningPolygon *>(), lines);

        setState(State::CanRunMission);
    }
}

void MissionPlanningContentCreator::runMission()
{
    notApi_.addNotification(new QLabel(QString("Starting Mission")));

    setState(State::CanCancelMission);
}

void MissionPlanningContentCreator::cancelMission()
{
    notApi_.addNotification(new QLabel(QString("Cancelling Mission")));

    setState(State::CanGetFlightPath);

    updatePois();
}

void MissionPlanningContentCreator::updatePois()
{
    MathExt().delay(20);

    auto points = poiApi_.pointsOfInterest();

    missionBounds_ = MathExt().findSmallestBoundingBox(points);

    pois_.clear();

    for (auto p : points)
        pois_.push_back({p.pointOfInterest().location()});

    if (pois_.size() > 2)
        setState(State::CanGetFlightPath);
    else
        setState(State::CannotGetFlightPath);

    drawMissionArea(points);
}

Q_SLOT void MissionPlanningContentCreator::drawMissionArea(QList<LmCdl::VcsiIdentifiedPointOfInterest> points)
{
    auto polygon = QGeoPolygon(missionBounds_.list());

    auto polygons = QList<MissionPlanningPolygon *>();

    polygons.append(new MissionPlanningPolygon(polygon));

    auto lines = QList<MissionPlanningLine *>();

    MathExt().cvhull(pois_);

    for (auto i = 1; i < pois_.size(); i++)
    {
        lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[i - 1][0]));
        if (i == pois_.size() - 1)
            lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[0][0]));
    }

    draw(polygons, lines);
}

void MissionPlanningContentCreator::draw(QList<MissionPlanningPolygon *> polygons, QList<MissionPlanningLine *> lines)
{
    drawing_->clear();
    drawing_->addPolygons(polygons);
    drawing_->addLines(lines);
    drawing_->update();

    drawApi_.removeDrawingForVectorData(*drawing_);
    drawApi_.addDrawingForVectorData(*drawing_, LmCdl::I_VectorDataDrawingApi::DrawingMode::OptimizedForFrequentChanges);
}

void MissionPlanningContentCreator::setState(State state)
{
    switch (state)
    {
    case CanGetFlightPath:
        missionBoundMenuItem_.setVisible(true);
        submitMissionMenuItem_.setVisible(true);
        submitMissionMenuItem_.setBackgroundColor(QColor(50, 100, 235, 180));
        submitMissionMenuItem_.setDescription("Get Flight Path");
        disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::cancelMission);
        disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::runMission);
        connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::getFlightPath);
        break;
    case CannotGetFlightPath:
        submitMissionMenuItem_.setVisible(false);
        break;
    case CanRunMission:
        missionBoundMenuItem_.setVisible(true);
        submitMissionMenuItem_.setVisible(true);
        submitMissionMenuItem_.setBackgroundColor(QColor(12, 235, 12, 180));
        submitMissionMenuItem_.setDescription("Begin Mission");
        disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::cancelMission);
        disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::getFlightPath);
        connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::runMission);
        break;
    case CanCancelMission:
        missionBoundMenuItem_.setVisible(false);
        submitMissionMenuItem_.setVisible(true);
        submitMissionMenuItem_.setBackgroundColor(QColor(235, 12, 12, 180));
        submitMissionMenuItem_.setDescription("Cancel Mission");
        disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::runMission);
        disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::getFlightPath);
        connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::cancelMission);
        break;
    };
}