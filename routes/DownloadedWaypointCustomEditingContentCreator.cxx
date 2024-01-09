/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <DownloadedWaypointCustomEditingContentCreator.h>

#include <DownloadedWaypointCustomEditingContent.h>
#include <LmCdl/I_RouteApi.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleCollectionApi.h>

DownloadedWaypointCustomEditingContentCreator::DownloadedWaypointCustomEditingContentCreator(
    LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi)
    : vehicleApi_(vehicleApi)
    , routeApi_(routeApi)
{
    connect(&vehicleApi, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this,
        &DownloadedWaypointCustomEditingContentCreator::makeDownloadedRouteContentForVehicle);
}

DownloadedWaypointCustomEditingContentCreator::~DownloadedWaypointCustomEditingContentCreator() = default;

void DownloadedWaypointCustomEditingContentCreator::makeDownloadedRouteContentForVehicle(
    LmCdl::UniqueIdentifier vehicleId)
{
    downloadedRouteContent_.emplace_back(new DownloadedWaypointCustomEditingContent(
        vehicleApi_.vehicle(vehicleId), routeApi_.downloadedRoutes(vehicleId)));
}
