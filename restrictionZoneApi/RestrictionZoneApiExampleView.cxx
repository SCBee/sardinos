/**
\file
Copyright (c) 2020 Lockheed Martin Corp.  All rights reserved.
*/

#include <RestrictionZoneApiExampleView.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiDockableExtensionApi.h>
#include <LmCdl/I_VcsiMenuBarExtensionApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QToolButton>
#include <QWidget>
#include <RestrictionZoneApiExampleContent.h>

RestrictionZoneApiExampleView::RestrictionZoneApiExampleView(
    LmCdl::I_VcsiApplicationApi& applicationApi, RestrictionZoneApiExampleContent& vectorDataRestrictionZones)
    : vectorDataRestrictionZones_(vectorDataRestrictionZones)

    , mainWidget_(new QWidget())
    , dockWidget_(new QDockWidget("Restriction Zone from Vector Data"))
    , showDockWidgetButton_(new QToolButton())
    , status_(new QTextEdit())
    , selectFile_(new QPushButton("Open File"))
    , queryZones_(new QPushButton("List Vector Data RZs"))
    , clearZones_(new QPushButton("Clear Vector Data Zones"))
{
    mainWidget_->setObjectName("Vector Data Restriction Zones");

    status_->setReadOnly(true);

    QGridLayout* mainLayout = new QGridLayout(mainWidget_);
    mainLayout->addWidget(status_, 0, 0, 1, 0);
    mainLayout->addWidget(selectFile_, 1, 0);
    mainLayout->addWidget(queryZones_, 1, 1);
    mainLayout->addWidget(clearZones_, 2, 0, 1, 0);

    showDockWidgetButton_->setCheckable(true);
    showDockWidgetButton_->setText("Vector Data RZ");
    showDockWidgetButton_->setToolTip("Vector Data RZ");
    applicationApi.widgetExtensionApi().menuBarApi().addItemToRightToolBar(showDockWidgetButton_);

    dockWidget_->setObjectName("Vector Data Restriction Zones");
    dockWidget_->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable
        | QDockWidget::DockWidgetFeature::DockWidgetFloatable | QDockWidget::DockWidgetFeature::DockWidgetClosable);
    dockWidget_->setAllowedAreas(Qt::DockWidgetArea::TopDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea
        | Qt::DockWidgetArea::LeftDockWidgetArea);
    dockWidget_->setWidget(mainWidget_);
    applicationApi.widgetExtensionApi().dockableApi().addDockWidget(
        Qt::DockWidgetArea::BottomDockWidgetArea, dockWidget_);

    connect(showDockWidgetButton_, &QAbstractButton::clicked, this,
        &RestrictionZoneApiExampleView::syncDockWidgetVisibility);
    connect(dockWidget_->toggleViewAction(), &QAction::toggled, showDockWidgetButton_, &QAbstractButton::setChecked);

    connect(
        selectFile_, &QAbstractButton::clicked, this, &RestrictionZoneApiExampleView::createRestrictionZonesFromFile);
    connect(queryZones_, &QAbstractButton::clicked, &vectorDataRestrictionZones_,
        &RestrictionZoneApiExampleContent::retrieveAndPrintRestrictionZonesCreatedByPlugin);
    connect(clearZones_, &QAbstractButton::clicked, &vectorDataRestrictionZones_,
        &RestrictionZoneApiExampleContent::removeRestrictionZonesCreatedByPlugin);

    connect(&vectorDataRestrictionZones_, &RestrictionZoneApiExampleContent::statusUpdate, this,
        [this](const QString& update) { status_->append(update); });
}

RestrictionZoneApiExampleView::~RestrictionZoneApiExampleView() { }

void RestrictionZoneApiExampleView::createRestrictionZonesFromFile()
{
    QFileDialog dialog;
    QFileInfo file(dialog.getOpenFileName(
        nullptr, tr("RestrictionZoneApiExamplePlugin - Load Vector Data"), "", tr("Vector Data (*.kml)")));
    if (file.exists()) {
        status_->append(QString("Loading file %1").arg(file.filePath()));
        vectorDataRestrictionZones_.loadVectorDataAndCreateRestrictionZones(file.filePath());
    }
}

void RestrictionZoneApiExampleView::syncDockWidgetVisibility(bool visible)
{
    if (visible) {
        dockWidget_->show();
        dockWidget_->raise();
    } else {
        dockWidget_->hide();
    }
}
