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
#include <LmCdl/AreaSearchInput.h>
#include <boost/optional.hpp>
#include <LmCdl/WrappedAnglePlusMinusPi.h>
#include <LmCdl/AngleUnit.h>
#include <LmCdl/Speed.h>
#include <qsize.h>
#include <LmCdl/Percentage.h>
#include <LmCdl/Frequency.h>

const auto SCANWIDTHMETERS = 20;
const auto MAXDISTANCEMETERS = 2000;

MissionPlanningContentCreator::MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi &mapApi,
                                                             LmCdl::I_PointOfInterestApi &poiApi,
                                                             LmCdl::I_VcsiUserNotificationApi &notApi,
                                                             LmCdl::I_VectorDataDrawingApi &drawApi,
                                                             LmCdl::I_AreaSearchApi &areaSearchApi)
    : missionBoundMenuItem_(mapApi.terrainContextMenu().registerMenuItem()), submitMissionMenuItem_(mapApi.terrainContextMenu().registerMenuItem()), poiApi_(poiApi), notApi_(notApi), drawApi_(drawApi), areaSearchApi_(areaSearchApi), notification_(nullptr)
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

    updateState();
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() = default;

void MissionPlanningContentCreator::connectToApiSignals()
{

    connect(&missionBoundMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
            &MissionPlanningContentCreator::getPoiProperties);

    connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
            &MissionPlanningContentCreator::getFlightPath);

    connect(&poiApi_, &LmCdl::I_PointOfInterestApi::pointOfInterestRemoved, this,
            &MissionPlanningContentCreator::updateState);

    connect(&areaSearchApi_, &LmCdl::I_AreaSearchApi::planningFailed, this, &MissionPlanningContentCreator::planningFail);
    connect(&areaSearchApi_, &LmCdl::I_AreaSearchApi::planningSucceeded, this, &MissionPlanningContentCreator::planningSuccess);
}

void MissionPlanningContentCreator::getPoiProperties(const LmCdl::ContextMenuEvent &event)
{

    const auto &location = event.worldLocation();

    auto properties = new LmCdl::VcsiPointOfInterestProperties(QString("Mission Bound"), location);

    auto id = new LmCdl::VcsiPointOfInterestId();

    publishAndMapPointOfInterest(*id, *properties);
}

void MissionPlanningContentCreator::removeNotification()
{

    notApi_.removeNotification(*notification_);
    notification_ = nullptr;
}

void MissionPlanningContentCreator::publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId sourceId,
                                                                 const LmCdl::VcsiPointOfInterestProperties &pointOfInterest)
{

    auto draw = [this, sourceId](const LmCdl::VcsiPointOfInterestId &cloneId)
    { updateState(); };

    poiApi_.addPointOfInterest(pointOfInterest, draw);
}

std::vector<double>
MissionPlanningContentCreator::sqPolar(QGeoCoordinate &point, QGeoCoordinate &com)
{

    double angle = atan2(point.latitude() - com.latitude(), point.longitude() - com.longitude());
    double distance = std::pow(point.longitude() - com.longitude(), 2) + std::pow(point.latitude() - com.latitude(), 2);

    return {angle, distance};
}

bool Comparator(const std::vector<QGeoCoordinate> &a, const std::vector<QGeoCoordinate> &b)
{

    if (a[1].longitude() != b[1].longitude())
    {
        return a[1].longitude() < b[1].longitude();
    }
    else
    {
        return a[1].latitude() < b[1].latitude();
    }
}

void MissionPlanningContentCreator::cvhull()
{

    double sumY = 0; // latitude
    double sumX = 0; // longitude
    for (const auto &poi : pois_)
    {
        sumX += poi[0].longitude(); // long
        sumY += poi[0].latitude();  // lat
    }
    double comLat = sumY / (double)pois_.size();  // y
    double comLong = sumX / (double)pois_.size(); // x
    auto com = QGeoCoordinate(comLat, comLong);

    for (auto &poi : pois_)
    {
        auto polarCoords = sqPolar(poi[0], com);
        auto tmp = QGeoCoordinate(polarCoords[1], polarCoords[0]);
        poi.push_back(tmp);
    }

    std::sort(pois_.begin(), pois_.end(), Comparator);
}

void MissionPlanningContentCreator::planningFail(LmCdl::AreaSearchPlanningId id, const QString &reason)
{
    notApi_.addNotification(new QLabel(QString(reason)));
}

void MissionPlanningContentCreator::planningSuccess(LmCdl::AreaSearchPlanningId id, const LmCdl::AreaSearchOutput &output)
{
    notApi_.addNotification(new QLabel(QString("SUCCESS")));
}

void MissionPlanningContentCreator::getFlightPath()
{
    updateState();

    auto searchInput = LmCdl::AreaSearchInput();

    auto alt = LmCdl::Altitude();

    alt = alt.withAltitude(LmCdl::Distance(100, LmCdl::DistanceUnit::Feet));

    searchInput = searchInput.withBoundary(QGeoPolygon(missionBounds_.list()));
    searchInput = searchInput.withDesiredAltitude(boost::optional<LmCdl::Altitude>(alt));
    searchInput = searchInput.withDesiredSpeed(boost::optional<LmCdl::Speed>(LmCdl::Speed(10, LmCdl::SpeedUnit::MetersPerSecond)));
    searchInput = searchInput.withCameraFieldOfViewHorizontal(boost::optional<LmCdl::WrappedAnglePlusMinusPi>(LmCdl::WrappedAnglePlusMinusPi(60, LmCdl::AngleUnit::Degrees)));
    searchInput = searchInput.withCameraFieldOfViewVertical(boost::optional<LmCdl::WrappedAnglePlusMinusPi>(LmCdl::WrappedAnglePlusMinusPi(60, LmCdl::AngleUnit::Degrees)));
    searchInput = searchInput.withCameraFrameRate(boost::optional<LmCdl::Frequency>(LmCdl::Frequency(60, LmCdl::FrequencyUnit::Hertz)));
    searchInput = searchInput.withCameraOverlapHorizontal(boost::optional<LmCdl::Percentage>(LmCdl::Percentage(2)));
    searchInput = searchInput.withCameraOverlapVertical(boost::optional<LmCdl::Percentage>(LmCdl::Percentage(2)));
    searchInput = searchInput.withCameraPixelResolution(boost::optional<QSize>(QSize(5, 5)));

    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.desiredAltitude().get().altitude().value(LmCdl::DistanceUnit::Feet)).c_str())));
    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.desiredSpeed().get().value(LmCdl::SpeedUnit::MetersPerSecond)).c_str())));
    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.cameraFieldOfViewHorizontal().get().value(LmCdl::AngleUnit::Degrees)).c_str())));
    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.cameraFieldOfViewVertical().get().value(LmCdl::AngleUnit::Degrees)).c_str())));
    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.cameraFrameRate().get().value(LmCdl::FrequencyUnit::Hertz)).c_str())));
    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.cameraOverlapHorizontal().get().value(LmCdl::PercentageUnit::OutOf100)).c_str())));
    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.cameraOverlapVertical().get().value(LmCdl::PercentageUnit::OutOf100)).c_str())));
    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.cameraPixelResolution().get().width()).c_str())));
    notApi_.addNotification(new QLabel(QString(std::to_string(searchInput.boundary().type()).c_str())));


    auto id = areaSearchApi_.startPlanning(searchInput);

    // auto flightPather = FlightPather(10, SCANWIDTHMETERS);

    // auto distance = 0.0;

    // QList<QGeoCoordinate> wayPoints;

    // if (missionBounds_.isVertical())
    //     wayPoints = flightPather.getVerticalFlightPath(missionBounds_);
    // else
    //     wayPoints = flightPather.getHorizontalFlightPath(missionBounds_);

    // auto polygons = QList<MissionPlanningPolygon *>();
    // auto lines = QList<MissionPlanningLine *>();
    // for (auto i = 0; i < wayPoints.size() - 1; i++)
    // {
    //     auto p1 = wayPoints[i];
    //     auto p2 = wayPoints[i + 1];
    //     lines.append(new MissionPlanningLine(p1, p2));
    //     distance += flightPather.getDistance(p1, p2);
    // }

    // if (distance > MAXDISTANCEMETERS)
    // {
    //     notApi_.addNotification(new QLabel(QString("Search area too big")));
    // }
    // else
    // {

    //     draw(polygons, lines);

    //     submitMissionMenuItem_.setBackgroundColor(QColor(12, 235, 12, 180));
    //     submitMissionMenuItem_.setDescription("Begin Mission");

    //     disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::getFlightPath);

    //     connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::beginMission);
    // }
}

void MissionPlanningContentCreator::beginMission()
{

    notApi_.addNotification(new QLabel(QString("Starting Mission")));

    missionBoundMenuItem_.setVisible(false);
    submitMissionMenuItem_.setBackgroundColor(QColor(235, 12, 12, 180));
    submitMissionMenuItem_.setDescription("Cancel Mission");

    disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::beginMission);

    connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::cancelMission);
}

void MissionPlanningContentCreator::cancelMission()
{

    notApi_.addNotification(new QLabel(QString("Cancelling Mission")));

    missionBoundMenuItem_.setVisible(true);
    submitMissionMenuItem_.setBackgroundColor(QColor(50, 100, 235, 180));
    submitMissionMenuItem_.setDescription("Get Flight Path");

    disconnect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::cancelMission);

    connect(&submitMissionMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this, &MissionPlanningContentCreator::getFlightPath);

    updateState();
}

void MissionPlanningContentCreator::updateState()
{

    delay(20);

    auto points = poiApi_.pointsOfInterest();

    missionBounds_ = findSmallestBoundingBox(points);

    pois_.clear();

    for (auto p : points)
        pois_.push_back({p.pointOfInterest().location()});

    if (pois_.size() > 2)
        submitMissionMenuItem_.setVisible(true);
    else
        submitMissionMenuItem_.setVisible(false);

    updateDrawing(points);
}

Q_SLOT void MissionPlanningContentCreator::updateDrawing(QList<LmCdl::VcsiIdentifiedPointOfInterest> points)
{

    auto polygon = new QGeoPolygon(missionBounds_.list());

    auto polygons = *new QList<MissionPlanningPolygon *>();

    polygons.append(new MissionPlanningPolygon(*polygon));

    auto lines = QList<MissionPlanningLine *>();

    cvhull();

    for (auto i = 1; i < pois_.size(); i++)
    {
        lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[i - 1][0]));
        if (i == pois_.size() - 1)
        {
            lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[0][0]));
        }
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
    drawApi_.addDrawingForVectorData(*drawing_,
                                     LmCdl::I_VectorDataDrawingApi::DrawingMode::OptimizedForFrequentChanges);
}

void MissionPlanningContentCreator::delay(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

BoundingBox MissionPlanningContentCreator::findSmallestBoundingBox(const QList<LmCdl::VcsiIdentifiedPointOfInterest> &points)
{

    if (points.empty())
        return BoundingBox();

    QGeoCoordinate southwest, northeast, southeast, northwest;
    southwest = northeast = southeast = northwest = points[0].pointOfInterest().location();

    for (const auto &point : points)
    {
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