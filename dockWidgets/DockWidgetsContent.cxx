/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <DockWidgetsContent.h>

#include <LmCdl/I_VcsiDockableExtensionApi.h>
#include <LmCdl/I_VcsiMenuBarExtensionApi.h>
#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QAction>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QToolButton>
#include <QWidget>

DockWidgetsContent::DockWidgetsContent(LmCdl::I_VcsiDockableExtensionApi& dockableApi,
    LmCdl::I_VcsiVehicleExtensionApi& vehicleApi, LmCdl::I_VcsiMenuBarExtensionApi& menuBarApi)
    : mainWidget_(new QWidget())
    , dockWidget_(new QDockWidget("Local Weather"))
    , weatherToday_(new QLabel())
    , weatherTommorrow_(new QLabel())
    , activeVehicleId_(new QLabel)
    , openDialogButton_(new QToolButton)
{
    mainWidget_->setObjectName("Weather Dialog");
    layoutContents();
    createAndRegisterDockWidget(dockableApi);
    createMenuBarButton(menuBarApi);
    lookupCurrentWeather();

    connect(&vehicleApi, &LmCdl::I_VcsiVehicleExtensionApi::activeVehicleChanged, this,
        &DockWidgetsContent::updateActiveVehicle);
}

void DockWidgetsContent::layoutContents()
{
    QGridLayout* layout = new QGridLayout(mainWidget_);

    layout->addWidget(new QLabel("Today"), 0, 0, Qt::AlignmentFlag::AlignCenter);
    layout->addWidget(weatherToday_, 1, 0, Qt::AlignmentFlag::AlignCenter);

    layout->setColumnMinimumWidth(1, 5);

    layout->addWidget(new QLabel("Tomorrow"), 0, 2, Qt::AlignmentFlag::AlignCenter);
    layout->addWidget(weatherTommorrow_, 1, 2, Qt::AlignmentFlag::AlignCenter);

    layout->addWidget(activeVehicleId_, 2, 0, 1, 3);
}

void DockWidgetsContent::createAndRegisterDockWidget(LmCdl::I_VcsiDockableExtensionApi& dockableApi)
{
    dockWidget_->setObjectName("Weather Dialog Dock Widget");
    dockWidget_->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable
        | QDockWidget::DockWidgetFeature::DockWidgetFloatable | QDockWidget::DockWidgetFeature::DockWidgetClosable);
    dockWidget_->setAllowedAreas(Qt::DockWidgetArea::TopDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea
        | Qt::DockWidgetArea::LeftDockWidgetArea);
    dockWidget_->setWidget(mainWidget_);

    dockableApi.addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dockWidget_);
}

void DockWidgetsContent::createMenuBarButton(LmCdl::I_VcsiMenuBarExtensionApi& menuBarApi)
{
    openDialogButton_->setCheckable(true);
    openDialogButton_->setIcon(QPixmap(":/weatherImages/WeatherIcon.png"));
    openDialogButton_->setToolTip("Local weather");
    menuBarApi.addItemToRightToolBar(openDialogButton_);

    connect(openDialogButton_, &QAbstractButton::clicked, this, &DockWidgetsContent::showOrHideWeatherDock);
    connect(dockWidget_->toggleViewAction(), &QAction::toggled, openDialogButton_, &QAbstractButton::setChecked);
}

void DockWidgetsContent::lookupCurrentWeather()
{
    QPixmap todayImage(":/weatherImages/Sunny.png");
    weatherToday_->setPixmap(todayImage);

    QPixmap tomorrowImage(":/weatherImages/Cloudy.png");
    weatherTommorrow_->setPixmap(tomorrowImage);
}

void DockWidgetsContent::updateActiveVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    activeVehicleId_->setText("Active vehicle ID: " + QString::number(vehicleId.value()));
}

void DockWidgetsContent::showOrHideWeatherDock(bool visible)
{
    if (visible) {
        dockWidget_->show();
        dockWidget_->raise();
    } else {
        dockWidget_->hide();
    }
}
