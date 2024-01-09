/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <TerrainProfileContent.h>

#include <LmCdl/GroundElevation.h>
#include <LmCdl/I_GroundElevationApi.h>
#include <LmCdl/I_MapPickApi.h>
#include <LmCdl/I_VcsiDockableExtensionApi.h>
#include <LmCdl/I_VcsiMenuBarExtensionApi.h>
#include <LmCdl/MapPickInformation.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QAction>
#include <QColor>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <QWidget>
#include <cmath>

namespace {
double angularDistanceBetweenLocations(const QGeoCoordinate& start, const QGeoCoordinate& end)
{
    double latitudeDelta = end.latitude() - start.latitude();
    double longitudeDelta = end.longitude() - start.longitude();
    return sqrt(latitudeDelta * latitudeDelta + longitudeDelta * longitudeDelta);
}
}

TerrainProfileContent::Renderer::Renderer(QGraphicsScene& scene)
    : scene_(scene)
    , xmax_(0)
    , ymax_(0)
    , flipAxes_(false)
    , pen_()
    , brush_(Qt::SolidPattern)
{
}

void TerrainProfileContent::Renderer::updateRenderParameters(const QSize& sceneSize)
{
    flipAxes_ = sceneSize.height() > sceneSize.width();
    xmax_ = flipAxes_ ? sceneSize.height() - 20 : sceneSize.width() - 20;
    ymax_ = flipAxes_ ? sceneSize.width() - 20 : sceneSize.height() - 20;
}

void TerrainProfileContent::Renderer::renderProfileLocation(
    const ProfileLocation& location, double profileLength, double maxHeight)
{
    const double x = location.distance / profileLength * xmax_ + 10;
    double y = maxHeight > 1e-12 ? location.height / maxHeight : 0.0;
    y = flipAxes_ ? y : 1.0 - y;
    y = y * ymax_ + 10;

    const QColor color = location.valid ? QColor("black") : QColor("red");
    pen_.setColor(color);
    brush_.setColor(color);
    auto drawEllipse = [this](double x, double y) { scene_.addEllipse(x, y, 3, 3, pen_, brush_); };
    flipAxes_ ? drawEllipse(y, x) : drawEllipse(x, y);
}

void TerrainProfileContent::Renderer::renderProfile(
    const std::vector<ProfileLocation>& profile, double profileLength, double maxHeight)
{
    scene_.clear();

    if (profile.empty()) {
        return;
    }

    if (profileLength < 1e-12) {
        scene_.addSimpleText(QString("End cannot equal start"));
        return;
    }

    for (const ProfileLocation& location : profile) {
        renderProfileLocation(location, profileLength, maxHeight);
    }
}

CoordinateEntry::CoordinateEntry(LmCdl::I_MapPickApi& mapPickApi, const QString& label)
    : mapPickApi(mapPickApi)
    , pickFromMap(new QPushButton(label))
    , latitude(new QDoubleSpinBox())
    , longitude(new QDoubleSpinBox())
{
    latitude->setRange(-90.0, 90.0);
    latitude->setDecimals(10);
    longitude->setRange(-180.0, 180.0);
    longitude->setDecimals(10);
    pickFromMap->setCheckable(true);
    connect(pickFromMap, &QAbstractButton::clicked, this, &CoordinateEntry::updatePickState);
}

void CoordinateEntry::addToLayout(QGridLayout& layout, int row)
{
    layout.addWidget(pickFromMap, row, 0, Qt::AlignmentFlag::AlignCenter);
    layout.addWidget(latitude, row, 1, Qt::AlignmentFlag::AlignCenter);
    layout.addWidget(longitude, row, 2, Qt::AlignmentFlag::AlignCenter);
}

void CoordinateEntry::updatePickState()
{
    if (pickFromMap->isChecked()) {
        using namespace std::placeholders;
        pickId = mapPickApi.requestMapPick(LmCdl::MapPickInformation(QCursor(Qt::CrossCursor),
                                               pickFromMap->text() + " Terrain Location", "Cancel", true),
            std::bind(&CoordinateEntry::updateCoordinate, this, _1), {},
            std::bind(&CoordinateEntry::cancelPick, this, _1));
    } else {
        mapPickApi.cancelMapPick(pickId);
    }
}

void CoordinateEntry::updateCoordinate(const LmCdl::MapPickResult& result)
{
    if (pickId.isValid() && pickId == result.requestId()) {
        pickId = LmCdl::MapPickRequestId();
        pickFromMap->setChecked(false);
        if (result.location().isValid()) {
            latitude->setValue(result.location().latitude());
            longitude->setValue(result.location().longitude());
        }
    }
}

void CoordinateEntry::cancelPick(const LmCdl::MapPickRequestId& id)
{
    if (pickId.isValid() && pickId == id) {
        pickId = LmCdl::MapPickRequestId();
        pickFromMap->setChecked(false);
    }
}

TerrainProfileContent::TerrainProfileContent(LmCdl::I_VcsiDockableExtensionApi& dockableApi,
    LmCdl::I_VcsiMenuBarExtensionApi& menuBarApi, LmCdl::I_GroundElevationApi& groundElevationApi,
    LmCdl::I_MapPickApi& mapPickApi)
    : groundElevationApi_(groundElevationApi)

    , mainWidget_(new QWidget())
    , dockWidget_(new QDockWidget("Terrain Profile"))

    , start_(mapPickApi, "Pick Start")
    , end_(mapPickApi, "Pick End")

    , sampleCount_(new QSpinBox())
    , synchronousUpdate_(new QPushButton("Update (sync)"))

    , profileScene_(new QGraphicsScene())
    , profileView_(new QGraphicsView())

    , openDialogButton_(new QToolButton)

    , profileLength_(-1.0)
    , maxHeight_(-1.0)
    , asyncRequestsOutstanding_(0u)

    , renderer_(*profileScene_)
{
    mainWidget_->setObjectName("Terrain Profile");
    layoutContents();
    createAndRegisterDockWidget(dockableApi);
    createMenuBarButton(menuBarApi);

    connect(synchronousUpdate_, &QAbstractButton::clicked, this, &TerrainProfileContent::synchronousUpdate);

    showOrHideDockWidget(false);

    profileView_->installEventFilter(this);
}

TerrainProfileContent::~TerrainProfileContent() { }

bool TerrainProfileContent::eventFilter(QObject* object, QEvent* event)
{
    if ((object == profileView_) && (event->type() == QEvent::Resize)) {
        renderer_.updateRenderParameters(profileView_->size());
        renderer_.renderProfile(profile_, profileLength_, maxHeight_);
    }
    return false;
}

void TerrainProfileContent::layoutContents()
{
    profileView_->setScene(profileScene_.data());
    profileView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    profileView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGridLayout* layout = new QGridLayout(mainWidget_);

    layout->addWidget(new QLabel("Latitude"), 0, 1, Qt::AlignmentFlag::AlignCenter);
    layout->addWidget(new QLabel("Longitude"), 0, 2, Qt::AlignmentFlag::AlignCenter);

    start_.addToLayout(*layout, 1);
    end_.addToLayout(*layout, 2);

    layout->addWidget(new QLabel("Sample Count"), 3, 0, Qt::AlignmentFlag::AlignRight);
    layout->addWidget(sampleCount_, 3, 1, Qt::AlignmentFlag::AlignCenter);

    layout->addWidget(synchronousUpdate_, 4, 1, Qt::AlignmentFlag::AlignCenter);
    layout->addWidget(profileView_, 5, 0, 1, 3);
    layout->setRowStretch(5, 1);

    sampleCount_->setRange(1, 1000000000);
    sampleCount_->setValue(100);
}

void TerrainProfileContent::createAndRegisterDockWidget(LmCdl::I_VcsiDockableExtensionApi& dockableApi)
{
    dockWidget_->setObjectName("Terrain Profile Dock Widget");
    dockWidget_->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable
        | QDockWidget::DockWidgetFeature::DockWidgetFloatable | QDockWidget::DockWidgetFeature::DockWidgetClosable);
    dockWidget_->setAllowedAreas(Qt::DockWidgetArea::TopDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea
        | Qt::DockWidgetArea::LeftDockWidgetArea);
    dockWidget_->setWidget(mainWidget_);

    dockableApi.addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dockWidget_);
}

void TerrainProfileContent::createMenuBarButton(LmCdl::I_VcsiMenuBarExtensionApi& menuBarApi)
{
    openDialogButton_->setCheckable(true);
    openDialogButton_->setText("Terrain Profile");
    openDialogButton_->setToolTip("Terrain Profile");
    menuBarApi.addItemToRightToolBar(openDialogButton_);

    connect(openDialogButton_, &QAbstractButton::clicked, this, &TerrainProfileContent::showOrHideDockWidget);
    connect(dockWidget_->toggleViewAction(), &QAction::toggled, openDialogButton_, &QAbstractButton::setChecked);
}

void TerrainProfileContent::showOrHideDockWidget(bool visible)
{
    if (visible) {
        dockWidget_->show();
        dockWidget_->raise();
    } else {
        dockWidget_->hide();
    }
}

void TerrainProfileContent::synchronizeButtonToggleState(bool dialogVisible)
{
    openDialogButton_->setChecked(dialogVisible);
}

void TerrainProfileContent::synchronousUpdate()
{
    QGeoCoordinate start = startLocation();
    QGeoCoordinate end = endLocation();
    initializeTerrainProfile(start, end);

    std::vector<QGeoCoordinate> interpolation = interpolate(start, end, sampleCount_->value() + 1);
    retrieveElevationSynchronously(start, interpolation);
    renderer_.renderProfile(profile_, profileLength_, maxHeight_);
}

QGeoCoordinate TerrainProfileContent::startLocation()
{
    return QGeoCoordinate(start_.latitude->value(), start_.longitude->value());
}

QGeoCoordinate TerrainProfileContent::endLocation()
{
    return QGeoCoordinate(end_.latitude->value(), end_.longitude->value());
}

void TerrainProfileContent::initializeTerrainProfile(const QGeoCoordinate& start, const QGeoCoordinate& end)
{
    clearTerrainProfile();

    profileLength_ = angularDistanceBetweenLocations(start, end);
}

std::vector<QGeoCoordinate> TerrainProfileContent::interpolate(QGeoCoordinate start, QGeoCoordinate end, int numSamples)
{
    std::vector<QGeoCoordinate> interpolation;
    interpolation.push_back(start);
    double startLatitude = start.latitude();
    double startLongitude = start.longitude();
    double endLatitude = end.latitude();
    double endLongitude = end.longitude();
    double latitudeDelta = (endLatitude - startLatitude) / numSamples;
    double longitudeDelta = (endLongitude - startLongitude) / numSamples;
    for (int i = 1; i < numSamples; ++i) {
        double latitude = startLatitude + i * latitudeDelta;
        double longitude = startLongitude + i * longitudeDelta;
        QGeoCoordinate interpolated(latitude, longitude);
        interpolation.push_back(interpolated);
    }

    interpolation.push_back(end);
    return interpolation;
}

void TerrainProfileContent::retrieveElevationSynchronously(
    QGeoCoordinate start, const std::vector<QGeoCoordinate>& samplePoints)
{
    for (const QGeoCoordinate& location : samplePoints) {
        LmCdl::GroundElevation elevation = groundElevationApi_.lookupGroundElevationSynchronously(location);
        addElevationToProfile(start, location, elevation);
    }
}

void TerrainProfileContent::addElevationToProfile(
    const QGeoCoordinate& start, const QGeoCoordinate& location, const LmCdl::GroundElevation& elevation)
{
    double distance = angularDistanceBetweenLocations(start, location);
    double height = elevation.altitudeMsl().value(LmCdl::DistanceUnit::Meters);
    if (height > maxHeight_) {
        maxHeight_ = height;
    }

    ProfileLocation profileLocation(height, distance, elevation.isValid());
    profile_.push_back(profileLocation);
}

void TerrainProfileContent::clearTerrainProfile()
{
    profileScene_->clear();
    profile_.clear();
    profileLength_ = -1.0;
    maxHeight_ = -1.0;
}

void TerrainProfileContent::enableProfileUpdates(bool enable) { synchronousUpdate_->setEnabled(enable); }
