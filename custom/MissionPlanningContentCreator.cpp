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
#include <MissionPlanningPolygonDrawing.h>
#include <MissionPlanningPolygon.h>
#include <LmCdl/I_VectorDataDrawingApi.h>

MissionPlanningContentCreator::MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi &mapApi,
                                                             LmCdl::I_PointOfInterestApi &poiApi,
                                                             LmCdl::I_VcsiUserNotificationApi &notApi,
                                                             LmCdl::I_VectorDataDrawingApi &drawApi)
        : contextMenuItem_(mapApi.terrainContextMenu().registerMenuItem()), poiApi_(poiApi), notApi_(notApi),
          drawApi_(drawApi), notification_(nullptr) {
    contextMenuItem_.setBackgroundColor(*new QColor(235, 12, 12, 180));
    contextMenuItem_.setDescription("Add Mission Bound");
    contextMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    contextMenuItem_.setIcon(":/MissionPlanning/missionPlanningDinoIcon");
    contextMenuItem_.setVisible(true);

    connectToApiSignals();

}

MissionPlanningContentCreator::~MissionPlanningContentCreator() = default;

void MissionPlanningContentCreator::connectToApiSignals() {
    connect(&contextMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
            &MissionPlanningContentCreator::getPoiProperties);

    connect(&poiApi_, &LmCdl::I_PointOfInterestApi::pointOfInterestAdded, this,
            &MissionPlanningContentCreator::testEmitSignal, Qt::QueuedConnection);

    connect(&poiApi_, &LmCdl::I_PointOfInterestApi::pointOfInterestRemoved, this,
            &MissionPlanningContentCreator::updatePolygon);
}

void MissionPlanningContentCreator::getPoiProperties(const LmCdl::ContextMenuEvent &event) {
    const auto &location = event.worldLocation();
    auto properties = new LmCdl::VcsiPointOfInterestProperties(*new QString("Mission Bound"), location);
    auto id = new LmCdl::VcsiPointOfInterestId();

    auto label = new QLabel();

    std::stringstream ss;
    ss << "Mission bound placed at " << std::fixed << std::setprecision(5) << location.latitude() << ", " << std::fixed
       << std::setprecision(5) << location.longitude();
    std::string xString = ss.str();
    label->setText(xString.c_str());

    auto removeTimer = std::make_unique<QTimer>();
    removeTimer->setInterval(3000);
    connect(removeTimer.get(), &QTimer::timeout, this, &MissionPlanningContentCreator::removeNotification);
    removeTimer->start();
    notification_ = &notApi_.addNotification(label);

    publishAndMapPointOfInterest(*id, *properties);
}

void MissionPlanningContentCreator::removeNotification() {
    notApi_.removeNotification(*notification_);
    notification_ = nullptr;
}

void MissionPlanningContentCreator::publishAndMapPointOfInterest(const LmCdl::VcsiPointOfInterestId &sourceId,
                                                                 const LmCdl::VcsiPointOfInterestProperties &pointOfInterest) {
    auto callback = [this, sourceId](const LmCdl::VcsiPointOfInterestId &cloneId) {};

    poiApi_.addPointOfInterest(pointOfInterest, callback);
    pois_.insert(sourceId, pointOfInterest);

    emit poiApi_.pointOfInterestAdded(sourceId, pointOfInterest);
}

void MissionPlanningContentCreator::testEmitSignal() {
    QTimer::singleShot(50, this, SLOT(updatePolygon()));
}

Q_SLOT void MissionPlanningContentCreator::updatePolygon() {
    auto polygon = new QGeoPolygon();
    auto points = poiApi_.pointsOfInterest();

    for (const auto &p: points) {
        polygon->addCoordinate(p.pointOfInterest().location());
    }
    auto missionPolygon = new MissionPlanningPolygon(*polygon);

    drawing_->clear();
    drawing_->addPolygon(missionPolygon);
    drawing_->update();

    drawApi_.removeDrawingForVectorData(*drawing_);
    drawApi_.addDrawingForVectorData(*drawing_,
                                     LmCdl::I_VectorDataDrawingApi::DrawingMode::OptimizedForFrequentChanges);
}