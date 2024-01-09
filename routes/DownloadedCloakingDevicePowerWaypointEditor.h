/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QJsonObject>
#include <QObject>
class CloakingDevicePower;
namespace LmCdl {
class I_DownloadedWaypointEditor;
}

class DownloadedCloakingDevicePowerWaypointEditor : public QObject {
public:
    DownloadedCloakingDevicePowerWaypointEditor(
        const CloakingDevicePower& cloakingDevicePower, LmCdl::I_DownloadedWaypointEditor& routeApi);
    virtual ~DownloadedCloakingDevicePowerWaypointEditor();

private:
    void setCloakingDeviceWaypointProperty(LmCdl::UniqueIdentifier waypoint);
    QJsonObject makeCloakingDeviceJson(LmCdl::UniqueIdentifier waypoint) const;

private:
    Q_DISABLE_COPY(DownloadedCloakingDevicePowerWaypointEditor)

    const CloakingDevicePower& cloakingDevicePower_;
    LmCdl::I_DownloadedWaypointEditor& waypointEditor_;
};
