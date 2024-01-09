/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <CustomRoutePropertiesContent.h>

#include <LmCdl/I_RouteApi.h>

CustomRoutePropertiesContent::CustomRoutePropertiesContent(
    LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi)
    : plannedContent_(routeApi)
    , downloadedContentCreator_(vehicleApi, routeApi)
    , routeUploadContentCreator_(vehicleApi, routeApi)
{
}

CustomRoutePropertiesContent::~CustomRoutePropertiesContent() { }
