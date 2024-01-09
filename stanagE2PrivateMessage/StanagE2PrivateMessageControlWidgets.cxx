/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <StanagE2PrivateMessageControlWidgets.h>

#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/TabWidgetMetadata.h>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

StanagE2PrivateMessageControlWidgets::StanagE2PrivateMessageControlWidgets(
    LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiVehicleExtensionApi& widgetApi)
    : mainWidget_(new QWidget())

    , horizontalPolarity_(new QPushButton("Horizontal"))
    , verticalPolarity_(new QPushButton("Vertical"))
    , crossedPolarity_(new QPushButton("Crossed"))

    , reportedPolarityLabel_(new QLabel("Reported Polarity:"))
    , reportedPolarity_(new QLabel())
{
    reportedPolarity_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layoutWidgets();
    registerTabWithApi(vehicleId, widgetApi);
}

void StanagE2PrivateMessageControlWidgets::layoutWidgets()
{
    QGridLayout* layout = new QGridLayout();

    layout->addWidget(horizontalPolarity_, 0, 0);
    layout->addWidget(verticalPolarity_, 0, 1);
    layout->addWidget(crossedPolarity_, 0, 2);

    layout->addWidget(reportedPolarityLabel_, 1, 0);
    layout->addWidget(reportedPolarity_, 1, 1, 1, -1);

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addItem(verticalSpacer, 2, 0, 1, -1);

    mainWidget_->setLayout(layout);
}

void StanagE2PrivateMessageControlWidgets::registerTabWithApi(
    LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiVehicleExtensionApi& widgetApi)
{
    const LmCdl::TabWidgetMetadata tabMetadata("Deflector");

    widgetApi.addSettingsMenu(vehicleId, tabMetadata, mainWidget_);
}

QPushButton* StanagE2PrivateMessageControlWidgets::horizontalPolarity() { return horizontalPolarity_; }

QPushButton* StanagE2PrivateMessageControlWidgets::verticalPolarity() { return verticalPolarity_; }

QPushButton* StanagE2PrivateMessageControlWidgets::crossedPolarity() { return crossedPolarity_; }

QLabel* StanagE2PrivateMessageControlWidgets::reportedPolarity() { return reportedPolarity_; }
