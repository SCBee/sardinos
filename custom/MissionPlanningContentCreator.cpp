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
#include <FlightPather.h>

MissionPlanningContentCreator::MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi &mapApi,
                                                             LmCdl::I_PointOfInterestApi &poiApi,
                                                             LmCdl::I_VcsiUserNotificationApi &notApi,
                                                             LmCdl::I_VectorDataDrawingApi &drawApi)
        : missionBoundMenuItem_(mapApi.terrainContextMenu().registerMenuItem()),
          submitMissionMenuItem_(mapApi.terrainContextMenu().registerMenuItem()), poiApi_(poiApi), notApi_(notApi),
          drawApi_(drawApi), notification_(nullptr) {

    missionBoundMenuItem_.setBackgroundColor(QColor(235, 12, 12, 180));
    missionBoundMenuItem_.setDescription("Add Mission Bound");
    missionBoundMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    missionBoundMenuItem_.setIcon(":/MissionPlanning/missionPlanningDinoIcon");
    missionBoundMenuItem_.setVisible(true);

    submitMissionMenuItem_.setBackgroundColor(QColor(12, 235, 12, 180));
    submitMissionMenuItem_.setDescription("Submit Mission");
    submitMissionMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Top);
    submitMissionMenuItem_.setIcon(":/MissionPlanning/UCIcon");
    submitMissionMenuItem_.setVisible(false);

    connectToApiSignals();

    updateState();
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() = default;

void MissionPlanningContentCreator::connectToApiSignals() {

    connect(&missionBoundMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
            &MissionPlanningContentCreator::getPoiProperties);

    connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
            &MissionPlanningContentCreator::getFlightPath);

    connect(&poiApi_, &LmCdl::I_PointOfInterestApi::pointOfInterestRemoved, this,
            &MissionPlanningContentCreator::updateState);
}

void MissionPlanningContentCreator::getPoiProperties(const LmCdl::ContextMenuEvent &event) {

    const auto &location = event.worldLocation();

    auto properties = new LmCdl::VcsiPointOfInterestProperties(QString("Mission Bound"), location);

    auto id = new LmCdl::VcsiPointOfInterestId();

    publishAndMapPointOfInterest(*id, *properties);
}

void MissionPlanningContentCreator::removeNotification() {

    notApi_.removeNotification(*notification_);
    notification_ = nullptr;
}

void MissionPlanningContentCreator::publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId sourceId,
                                                                 const LmCdl::VcsiPointOfInterestProperties &pointOfInterest) {

    auto draw = [this, sourceId](const LmCdl::VcsiPointOfInterestId &cloneId) { updateState(); };

    poiApi_.addPointOfInterest(pointOfInterest, draw);
}

std::vector<double>
MissionPlanningContentCreator::sqPolar(QGeoCoordinate &point, QGeoCoordinate &com) {

    double angle = atan2(point.latitude() - com.latitude(), point.longitude() - com.longitude());
    double distance = std::pow(point.longitude() - com.longitude(), 2) + std::pow(point.latitude() - com.latitude(), 2);

    return {angle, distance};
}

bool
Comparator(const std::vector<QGeoCoordinate> &a, const std::vector<QGeoCoordinate> &b) {

    if (a[1].longitude() != b[1].longitude()) {
        return a[1].longitude() < b[1].longitude();
    } else {
        return a[1].latitude() < b[1].latitude();
    }
}

void MissionPlanningContentCreator::cvhull() {

    double sumY = 0; // latitude
    double sumX = 0; // longitude
    for (const auto &poi: pois_) {
        sumX += poi[0].longitude(); // long
        sumY += poi[0].latitude(); // lat
    }
    double comLat = sumY / (double) pois_.size(); // y
    double comLong = sumX / (double) pois_.size(); // x
    auto com = QGeoCoordinate(comLat, comLong);

    for (auto &poi: pois_) {
        auto polarCoords = sqPolar(poi[0], com);
        auto tmp = QGeoCoordinate(polarCoords[1], polarCoords[0]);
        poi.push_back(tmp);
    }

    std::sort(pois_.begin(), pois_.end(), Comparator);
}

void MissionPlanningContentCreator::getFlightPath() {

    updateState();

    auto flightPather = FlightPather(10, 20);

    auto longSpread = flightPather.getDistance(missionBounds_.SW, missionBounds_.SE);

    auto latSpread = flightPather.getDistance(missionBounds_.SW, missionBounds_.NW);

    notApi_.addNotification(new QLabel(QString(("long spread: " + std::to_string(longSpread)).c_str())));
    notApi_.addNotification(new QLabel(QString(("lat spread: " + std::to_string(latSpread)).c_str())));
}

void MissionPlanningContentCreator::updateState() {

    delay(20);

    auto points = poiApi_.pointsOfInterest();

    missionBounds_ = findSmallestBoundingBox(points);

    pois_.clear();

    for (auto p: points) pois_.push_back({p.pointOfInterest().location()});

    if (pois_.size() > 2) submitMissionMenuItem_.setVisible(true);
    else submitMissionMenuItem_.setVisible(false);

    updateDrawing(points);
}

Q_SLOT void MissionPlanningContentCreator::updateDrawing(QList<LmCdl::VcsiIdentifiedPointOfInterest> points) {

    auto polygon = new QGeoPolygon(missionBounds_.list());

    auto polygons = *new QList<MissionPlanningPolygon *>();

    polygons.append(new MissionPlanningPolygon(*polygon));

    auto lines = QList<MissionPlanningLine *>();


    cvhull();

    for (auto i = 1; i < pois_.size(); i++) {
        lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[i - 1][0]));
        if (i == pois_.size() - 1) {
            lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[0][0]));
        }
    }

    draw(polygons, lines);
}

void MissionPlanningContentCreator::draw(QList<MissionPlanningPolygon *> polygons, QList<MissionPlanningLine *> lines) {
    drawing_->clear();
    drawing_->addPolygons(polygons);
    drawing_->addLines(lines);
    drawing_->update();

    drawApi_.removeDrawingForVectorData(*drawing_);
    drawApi_.addDrawingForVectorData(*drawing_,
                                     LmCdl::I_VectorDataDrawingApi::DrawingMode::OptimizedForFrequentChanges);
}

void MissionPlanningContentCreator::delay(int ms) {
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

BoundingBox
MissionPlanningContentCreator::findSmallestBoundingBox(const QList<LmCdl::VcsiIdentifiedPointOfInterest> &points) {

    if (points.empty()) return {};

    QGeoCoordinate southwest, northeast, southeast, northwest;
    southwest = northeast = southeast = northwest = points[0].pointOfInterest().location();

    for (const auto &point: points) {
        southwest.setLatitude(std::min(southwest.latitude(), point.pointOfInterest().location().latitude()));
        southwest.setLongitude(std::min(southwest.longitude(), point.pointOfInterest().location().longitude()));

        northeast.setLatitude(std::max(northeast.latitude(), point.pointOfInterest().location().latitude()));
        northeast.setLongitude(std::max(northeast.longitude(), point.pointOfInterest().location().longitude()));
    }

    southeast.setLatitude(southwest.latitude());
    southeast.setLongitude(northeast.longitude());

    northwest.setLatitude(northeast.latitude());
    northwest.setLongitude(southwest.longitude());

    return {southwest, northwest, southeast, northeast};
}