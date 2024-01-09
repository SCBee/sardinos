/**
\file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <FaaObstacleContent.h>

#include <FaaObstacleInfo.h>
#include <FaaObstacleSource.h>
#include <LmCdl/Distance.h>
#include <LmCdl/I_PanterraPolygon.h>
#include <LmCdl/I_PanterraPolygonApi.h>
#include <LmCdl/I_PanterraPublicApi.h>
#include <LmCdl/I_PanterraShapeApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiGlobalSettingsExtensionApi.h>
#include <LmCdl/I_VcsiMapLayer.h>
#include <LmCdl/I_VcsiMapLayerApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/TabWidgetMetadata.h>
#include <LmCdl/WrappedAnglePlusMinusPi.h>
#include <QColor>
#include <QFile>
#include <QFileDialog>
#include <QGeoCoordinate>
#include <QPushButton>
#include <QWidget>
#include <QtMath>
#include <QtPlugin>
#include <math.h>
#include <ui_FaaObstacleForm.h>

FaaObstacleContent::FaaObstacleContent(LmCdl::I_PanterraPublicApi& panterraApi, LmCdl::I_VcsiApplicationApi& guiApi)
    : panterraApi_(panterraApi)
    , guiApi_(guiApi)
    , obstacleMapLayer_(guiApi_.mapLayerApi().ensureMapLayerExists("Overlays/FAA Obstacles"))
    , obstacleVisibility_(true)
{
    QWidget* settingsWidget = new QWidget;
    Ui::ObstacleForm* settingsForm = new Ui::ObstacleForm;
    settingsForm->setupUi(settingsWidget);
    filePathInput_ = settingsForm->dofFilePathEdit;

    connect(settingsForm->browseButton, &QPushButton::clicked, this, &FaaObstacleContent::browseForFile);
    connect(settingsForm->loadObstacleButton, &QPushButton::clicked, this, &FaaObstacleContent::loadObstacleFile);
    connect(settingsForm->clearButton, &QPushButton::clicked, this, &FaaObstacleContent::clearObstacles);

    LmCdl::TabWidgetMetadata tabData("Obstacles");
    guiApi_.widgetExtensionApi().globalSettingsApi().addSettingsMenu(tabData, settingsWidget);

    connect(&obstacleMapLayer_, &LmCdl::I_VcsiMapLayer::visibilityChanged, this,
        &FaaObstacleContent::updateObstacleVisibility);
}

FaaObstacleContent::~FaaObstacleContent() { }

void FaaObstacleContent::browseForFile()
{
    const QString windowTitle = QStringLiteral("Choose Obstacle File");
    const QString startingDirectory = QDir::homePath();
    const QString fileFilter = QStringLiteral("*.dat");

    const QString fileName = QFileDialog::getOpenFileName(nullptr, windowTitle, startingDirectory, fileFilter);
    if (!fileName.isEmpty()) {
        filePathInput_->setText(fileName);
    }
}

void FaaObstacleContent::loadObstacleFile()
{
    FaaObstacleSource obstacleSource(filePathInput_->text());
    QList<FaaObstacleInfo> obstacles = obstacleSource.obstacles();
    createSpheresAtObstacles(obstacles);
}

void FaaObstacleContent::clearObstacles()
{
    foreach (LmCdl::I_PanterraShape* shape, shapeList_) {
        panterraApi_.shapeApi().removeShape(*shape);
    }
    shapeList_.clear();
}

void FaaObstacleContent::createSpheresAtObstacles(const QList<FaaObstacleInfo>& obstacles)
{
    foreach (FaaObstacleInfo obstacle, obstacles) {
        double groundLevelMsl = obstacle.mslHeight.value(LmCdl::DistanceUnit::Meters)
            - obstacle.aglHeight.value(LmCdl::DistanceUnit::Meters);

        LmCdl::I_PanterraShape& shape = panterraApi_.shapeApi().createShape(panterraApi_.polygonApi().createCylinder(
            qDegreesToRadians(obstacle.location.latitude()), qDegreesToRadians(obstacle.location.longitude()),
            groundLevelMsl, qDegreesToRadians(obstacle.location.latitude()),
            qDegreesToRadians(obstacle.location.longitude()), obstacle.mslHeight.value(LmCdl::DistanceUnit::Meters),
            obstacle.horizontalAccuracy.value(LmCdl::DistanceUnit::Meters)));
        shapeList_.append(&shape);

        if (obstacle.dismantled) {
            shape.setColor(QColor(255, 0, 0, 120));
        } else if (!obstacle.verified) {
            shape.setColor(QColor(0, 0, 255, 120));
        } else {
            shape.setColor(QColor(205, 155, 29, 200));
        }

        shape.setVisible(obstacleVisibility_);
    }
}

void FaaObstacleContent::updateObstacleVisibility(bool visibility)
{
    obstacleVisibility_ = visibility;

    foreach (LmCdl::I_PanterraShape* shape, shapeList_) {
        shape->setVisible(obstacleVisibility_);
    }
}
