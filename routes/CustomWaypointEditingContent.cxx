/**
\file
Copyright (c) 2017 Lockheed Martin Corp.  All rights reserved.
*/

#include <CustomWaypointEditingContent.h>

#include <LmCdl/I_RouteApiPlugin.h>
#include <LmCdl/I_VehicleCollectionApiPlugin.h>

CustomWaypointEditingContent::CustomWaypointEditingContent(
    LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi)
    : plannedContent_(routeApi)
    , downloadedContentCreator_(vehicleApi, routeApi)
{
}

CustomWaypointEditingContent::~CustomWaypointEditingContent() { }
