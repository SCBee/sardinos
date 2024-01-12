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
#include <algorithm>

MissionPlanningContentCreator::MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi &mapApi,
                                                             LmCdl::I_PointOfInterestApi &poiApi,
                                                             LmCdl::I_VcsiUserNotificationApi &notApi,
                                                             LmCdl::I_VectorDataDrawingApi &drawApi)
        : contextMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
        , poiApi_(poiApi)
        , notApi_(notApi)
        , drawApi_(drawApi)
        , notification_(nullptr) 
{
    contextMenuItem_.setBackgroundColor(*new QColor(235, 12, 12, 180));
    contextMenuItem_.setDescription("Add Mission Bound");
    contextMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    contextMenuItem_.setIcon(":/MissionPlanning/missionPlanningDinoIcon");
    contextMenuItem_.setVisible(true);

    connectToApiSignals();

    updateDrawing();
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() = default;

void MissionPlanningContentCreator::connectToApiSignals() {
    connect(&contextMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
            &MissionPlanningContentCreator::getPoiProperties);

    connect(&poiApi_, &LmCdl::I_PointOfInterestApi::pointOfInterestRemoved, this,
            &MissionPlanningContentCreator::updateDrawing);
}

void MissionPlanningContentCreator::getPoiProperties(const LmCdl::ContextMenuEvent &event) {
    const auto &location = event.worldLocation();

    auto properties = new LmCdl::VcsiPointOfInterestProperties(*new QString("Mission Bound"), location);

    auto id = new LmCdl::VcsiPointOfInterestId();

    publishAndMapPointOfInterest(*id, *properties);
}

void MissionPlanningContentCreator::removeNotification() {
    notApi_.removeNotification(*notification_);
    notification_ = nullptr;
}

void MissionPlanningContentCreator::publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId sourceId,
                                                                 const LmCdl::VcsiPointOfInterestProperties &pointOfInterest) {
    auto mapIds = [this, sourceId](const LmCdl::VcsiPointOfInterestId &cloneId) {};

    poiApi_.addPointOfInterest(pointOfInterest, mapIds);

    pois_.insert(sourceId, pointOfInterest);

    updateDrawing();
}

Q_SLOT void MissionPlanningContentCreator::updateDrawing() {
    
    delay(20);

    auto points = poiApi_.pointsOfInterest();

    auto polygon = new QGeoPolygon(findSmallestBoundingBox(points));

    auto polygons = *new QList<MissionPlanningPolygon*>();

    polygons.append(new MissionPlanningPolygon(*polygon));

    auto lines = *new QList<MissionPlanningLine*>();

    for (auto i = 0; i < points.size(); i ++) {
        if (i == points.size() - 1)
            lines.append(new MissionPlanningLine(points[i].pointOfInterest().location(), points[0].pointOfInterest().location()));
        else 
            lines.append(new MissionPlanningLine(points[i].pointOfInterest().location(), points[i+1].pointOfInterest().location()));
    }

    draw(polygons, lines);
}

void MissionPlanningContentCreator::draw(QList<MissionPlanningPolygon*> polygons, QList<MissionPlanningLine*> lines){
    drawing_->clear();
    drawing_->addPolygons(polygons);
    drawing_->addLines(lines);
    drawing_->update();

    drawApi_.removeDrawingForVectorData(*drawing_);
    drawApi_.addDrawingForVectorData(*drawing_, LmCdl::I_VectorDataDrawingApi::DrawingMode::OptimizedForFrequentChanges);
}

void MissionPlanningContentCreator::delay(int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QList<QGeoCoordinate> MissionPlanningContentCreator::findSmallestBoundingBox(const QList<LmCdl::VcsiIdentifiedPointOfInterest>& points) {
    
    if (points.empty()) return QList<QGeoCoordinate>();
    
    QGeoCoordinate southwest, northeast, southeast, northwest;
    southwest = northeast = southeast = northwest = points[0].pointOfInterest().location(); 

    for (const auto& point : points) {
        southwest.setLatitude(std::min(southwest.latitude(), point.pointOfInterest().location().latitude()));
        southwest.setLongitude(std::min(southwest.longitude(), point.pointOfInterest().location().longitude()));

        northeast.setLatitude(std::max(northeast.latitude(), point.pointOfInterest().location().latitude()));
        northeast.setLongitude(std::max(northeast.longitude(), point.pointOfInterest().location().longitude()));
    }

    southeast.setLatitude(southwest.latitude());
    southeast.setLongitude(northeast.longitude());

    northwest.setLatitude(northeast.latitude());
    northwest.setLongitude(southwest.longitude());

    return  {southwest,  southeast, northeast, northwest};
}
