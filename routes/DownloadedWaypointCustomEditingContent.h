/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <CloakingDevicePower.h>
#include <DownloadedCloakingDevicePowerMessageReceiver.h>
#include <DownloadedCloakingDevicePowerView.h>
#include <DownloadedCloakingDevicePowerWaypointEditor.h>

namespace LmCdl {
class I_DownloadedRouteCollection;
class I_Vehicle;
}

class DownloadedWaypointCustomEditingContent {
public:
    explicit DownloadedWaypointCustomEditingContent(
        LmCdl::I_Vehicle& vehicle, LmCdl::I_DownloadedRouteCollection& routeApi);

private:
    Q_DISABLE_COPY(DownloadedWaypointCustomEditingContent)

    CloakingDevicePower cloakingDevicePower_;
    DownloadedCloakingDevicePowerMessageReceiver messageReceiver_;
    DownloadedCloakingDevicePowerWaypointEditor waypointEditor_;
    DownloadedCloakingDevicePowerView view_;
};
