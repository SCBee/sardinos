/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <PlannedWaypointCloakingDeviceMutator.h>

#include <CloakingDeviceJsonDefines.h>
#include <LmCdl/I_PlannedRouteCollection.h>
#include <LmCdl/I_PlannedWaypointEditor.h>
#include <LmCdl/StanagWaypoint.h>
#include <PlannedWaypointCustomEditingUi.h>
#include <QCheckBox>
#include <QJsonObject>

PlannedWaypointCloakingDeviceMutator::PlannedWaypointCloakingDeviceMutator(LmCdl::UniqueIdentifier waypointId,
    PlannedWaypointCustomEditingUi& ui, LmCdl::I_PlannedRouteCollection& routeCollection,
    LmCdl::I_PlannedWaypointEditor& waypointEditor)
    : waypointId_(waypointId)
    , ui_(ui)
    , routeCollection_(routeCollection)
{
    connect(&ui.cloakingDeviceEdit(), &QCheckBox::stateChanged, this,
        &PlannedWaypointCloakingDeviceMutator::tellVcsiCloakingDeviceChangesArePending);

    connect(&waypointEditor, &LmCdl::I_PlannedWaypointEditor::waypointChangesConfirmed, this,
        &PlannedWaypointCloakingDeviceMutator::commitPendingCloakingDevice);
    connect(&waypointEditor, &LmCdl::I_PlannedWaypointEditor::waypointChangesCancelled, this,
        &PlannedWaypointCloakingDeviceMutator::cancelPendingCloakingDevice);

    syncUiToCurrentCloakingDeviceState();
    commitPendingCloakingDevice(waypointId_);
}

PlannedWaypointCloakingDeviceMutator::~PlannedWaypointCloakingDeviceMutator() { }

void PlannedWaypointCloakingDeviceMutator::tellVcsiCloakingDeviceChangesArePending()
{
    routeCollection_.waypointEditor().setWaypointHasPendingChanges(waypointId_);
}

void PlannedWaypointCloakingDeviceMutator::commitPendingCloakingDevice(LmCdl::UniqueIdentifier waypointId)
{
    if (waypointId == waypointId_) {
        bool cloakingDevicePowerState = ui_.cloakingDeviceEdit().checkState();
        routeCollection_.waypointEditor().setCustomProperty(waypointId_, CloakingDeviceJsonDefines::CLOAKING_DEVICE_TAG,
            writePowerStateToJson(cloakingDevicePowerState));
    }
}

void PlannedWaypointCloakingDeviceMutator::cancelPendingCloakingDevice(LmCdl::UniqueIdentifier waypointId)
{
    if (waypointId == waypointId_) {
        syncUiToCurrentCloakingDeviceState();
    }
}

void PlannedWaypointCloakingDeviceMutator::syncUiToCurrentCloakingDeviceState()
{
    bool queriedState = queryCurrentCloakingDeviceState();
    QSignalBlocker block(ui_.cloakingDeviceEdit());
    ui_.cloakingDeviceEdit().setChecked(queriedState);
}

QJsonObject PlannedWaypointCloakingDeviceMutator::writePowerStateToJson(bool cloakingDevicePowerState) const
{
    QJsonObject json;
    json.insert(CloakingDeviceJsonDefines::POWER_TAG, powerStateToString(cloakingDevicePowerState));
    return json;
}

QString PlannedWaypointCloakingDeviceMutator::readPowerStringFromJson(const QJsonObject& json) const
{
    return json.value(CloakingDeviceJsonDefines::CLOAKING_DEVICE_TAG)
        .toObject()
        .value(CloakingDeviceJsonDefines::POWER_TAG)
        .toString();
}

QString PlannedWaypointCloakingDeviceMutator::powerStateToString(bool cloakingDevicePowerState) const
{
    return cloakingDevicePowerState ? CloakingDeviceJsonDefines::ON_VALUE : CloakingDeviceJsonDefines::OFF_VALUE;
}

bool PlannedWaypointCloakingDeviceMutator::powerStringToBool(const QString& powerString) const
{
    if (CloakingDeviceJsonDefines::ON_VALUE == powerString) {
        return true;
    }
    return false;
}

bool PlannedWaypointCloakingDeviceMutator::queryCurrentCloakingDeviceState() const
{
    LmCdl::StanagWaypoint waypoint = routeCollection_.lookupWaypoint(waypointId_);
    QJsonObject waypointCustomProperties = waypoint.customPropertyExtension;
    return powerStringToBool(readPowerStringFromJson(waypointCustomProperties));
}
