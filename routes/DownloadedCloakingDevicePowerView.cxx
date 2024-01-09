/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <DownloadedCloakingDevicePowerView.h>

#include <CloakingDevicePower.h>
#include <LmCdl/I_DownloadedRouteCollection.h>
#include <QBoxLayout>
#include <QLabel>

DownloadedCloakingDevicePowerView::DownloadedCloakingDevicePowerView(
    const CloakingDevicePower& cloakingDevicePower, LmCdl::I_DownloadedRouteCollection& routes)
    : cloakingDevicePower_(cloakingDevicePower)
    , routes_(routes)
{
    connect(&cloakingDevicePower_, &CloakingDevicePower::cloakingDevicePowerChanged, this,
        &DownloadedCloakingDevicePowerView::updateCloakingDeviceDisplay);
}

DownloadedCloakingDevicePowerView::~DownloadedCloakingDevicePowerView() { }

void DownloadedCloakingDevicePowerView::updateCloakingDeviceDisplay(LmCdl::UniqueIdentifier waypointId)
{
    if (!cloakingDeviceLabels_.contains(waypointId)) {
        createNewCloakingDeviceWidget(waypointId);
    }
    cloakingDeviceLabels_.value(waypointId)->setText(cloakingDevicePower_.cloakingDevicePowerToString(waypointId));
}

void DownloadedCloakingDevicePowerView::createNewCloakingDeviceWidget(LmCdl::UniqueIdentifier waypointId)
{
    QWidget* containterWidget = new QWidget;
    QLabel* cloakingPowerLabel = new QLabel;

    QLayout* layout = new QHBoxLayout(containterWidget);
    layout->addWidget(new QLabel("Cloaking Power:"));
    layout->addWidget(cloakingPowerLabel);
    layout->setContentsMargins(0, 0, 0, 0);

    cloakingDeviceLabels_.insert(waypointId, cloakingPowerLabel);
    routes_.addWaypointWidget(waypointId, containterWidget);
}
