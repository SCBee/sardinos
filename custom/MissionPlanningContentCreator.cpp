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

    auto label = new QLabel();

    std::stringstream ss;

    ss << "Mission bound placed at " << std::fixed << std::setprecision(5) << location.latitude() << ", " << std::fixed
       << std::setprecision(5) << location.longitude();

    std::string xString = ss.str();

    label->setText(xString.c_str());

    auto removeTimer = new QTimer();
    removeTimer->setInterval(100);
    connect(removeTimer, &QTimer::timeout, this, &MissionPlanningContentCreator::removeNotification);
    removeTimer->start();
    notification_ = &notApi_.addNotification(label);
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

void MissionPlanningContentCreator::removePoi(LmCdl::VcsiPointOfInterestId id) 
{
    pois_.remove(id);

    updateDrawing();
}

Q_SLOT void MissionPlanningContentCreator::updateDrawing() {
    
    delay(20);

    auto points = poiApi_.pointsOfInterest();

    auto lines = *new QList<MissionPlanningLine*>();

    for (auto i = 0; i < points.size(); i ++) {
        if (i == points.size() - 1)
            lines.append(new MissionPlanningLine(points[i].pointOfInterest().location(), points[0].pointOfInterest().location()));
        else 
            lines.append(new MissionPlanningLine(points[i].pointOfInterest().location(), points[i+1].pointOfInterest().location()));
    }

    drawing_->clear();
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