/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <DownloadedWaypointCustomEditingContent.h>

#include <LmCdl/I_DownloadedRouteCollection.h>

DownloadedWaypointCustomEditingContent::DownloadedWaypointCustomEditingContent(
    LmCdl::I_Vehicle& vehicle, LmCdl::I_DownloadedRouteCollection& routeApi)
    : cloakingDevicePower_()
    , messageReceiver_(vehicle, cloakingDevicePower_)
    , waypointEditor_(cloakingDevicePower_, routeApi.waypointEditor())
    , view_(cloakingDevicePower_, routeApi)
{
}
