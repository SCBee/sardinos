/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
class PlannedWaypointCustomEditingUi;
class QJsonObject;
namespace LmCdl {
class I_PlannedWaypointEditor;
class I_PlannedRouteCollection;
}

class PlannedWaypointCloakingDeviceMutator : public QObject {
public:
    explicit PlannedWaypointCloakingDeviceMutator(LmCdl::UniqueIdentifier waypointId,
        PlannedWaypointCustomEditingUi& ui, LmCdl::I_PlannedRouteCollection& routeCollection,
        LmCdl::I_PlannedWaypointEditor& waypointEditor);
    virtual ~PlannedWaypointCloakingDeviceMutator();

private:
    void tellVcsiCloakingDeviceChangesArePending();

    void commitPendingCloakingDevice(LmCdl::UniqueIdentifier waypointId);
    void cancelPendingCloakingDevice(LmCdl::UniqueIdentifier waypointId);

private:
    void syncUiToCurrentCloakingDeviceState();
    QJsonObject writePowerStateToJson(bool cloakingDevicePowerState) const;
    QString readPowerStringFromJson(const QJsonObject& json) const;
    QString powerStateToString(bool cloakingDevicePowerState) const;
    bool powerStringToBool(const QString& powerString) const;
    bool queryCurrentCloakingDeviceState() const;

    LmCdl::UniqueIdentifier waypointId_;
    PlannedWaypointCustomEditingUi& ui_;
    LmCdl::I_PlannedRouteCollection& routeCollection_;
};
