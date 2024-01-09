/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <CustomWaypointUploadContentCreator.h>

#include <CustomWaypointUploadContent.h>
#include <LmCdl/I_PlannedRouteCollection.h>
#include <LmCdl/I_RouteApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>

CustomWaypointUploadContentCreator::CustomWaypointUploadContentCreator(
    LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi)
    : vehicleApi_(vehicleApi)
    , routeApi_(routeApi)
{
    connect(&vehicleApi_, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this,
        &CustomWaypointUploadContentCreator::makeRouteUploadContentForVehicle);
}

CustomWaypointUploadContentCreator::~CustomWaypointUploadContentCreator() { }

void CustomWaypointUploadContentCreator::makeRouteUploadContentForVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    routeUploadContent_.emplace_back(
        new CustomWaypointUploadContent(routeApi_.plannedRoutes().waypointUploadSourceRegistry(vehicleId)));
}
