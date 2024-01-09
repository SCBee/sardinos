/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <DownloadedCloakingDevicePowerWaypointEditor.h>

#include <CloakingDeviceJsonDefines.h>
#include <CloakingDevicePower.h>
#include <LmCdl/I_DownloadedWaypointEditor.h>

DownloadedCloakingDevicePowerWaypointEditor::DownloadedCloakingDevicePowerWaypointEditor(
    const CloakingDevicePower& cloakingDevicePower, LmCdl::I_DownloadedWaypointEditor& waypointEditor)
    : cloakingDevicePower_(cloakingDevicePower)
    , waypointEditor_(waypointEditor)
{
    connect(&cloakingDevicePower_, &CloakingDevicePower::cloakingDevicePowerChanged, this,
        &DownloadedCloakingDevicePowerWaypointEditor::setCloakingDeviceWaypointProperty);
}

DownloadedCloakingDevicePowerWaypointEditor::~DownloadedCloakingDevicePowerWaypointEditor() { }

void DownloadedCloakingDevicePowerWaypointEditor::setCloakingDeviceWaypointProperty(LmCdl::UniqueIdentifier waypoint)
{
    waypointEditor_.setCustomProperty(
        waypoint, CloakingDeviceJsonDefines::CLOAKING_DEVICE_TAG, makeCloakingDeviceJson(waypoint));
}

QJsonObject DownloadedCloakingDevicePowerWaypointEditor::makeCloakingDeviceJson(LmCdl::UniqueIdentifier waypoint) const
{
    QJsonObject json;
    json.insert(CloakingDeviceJsonDefines::POWER_TAG, cloakingDevicePower_.cloakingDevicePowerToString(waypoint));
    return json;
}
