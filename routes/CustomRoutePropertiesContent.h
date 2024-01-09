/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <CustomWaypointUploadContentCreator.h>
#include <DownloadedWaypointCustomEditingContentCreator.h>
#include <LmCdl/I_RouteApiPlugin.h>
#include <LmCdl/I_VehicleCollectionApiPlugin.h>
#include <PlannedWaypointCustomEditingContent.h>

class CustomRoutePropertiesContent {
public:
    explicit CustomRoutePropertiesContent(LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi);
    virtual ~CustomRoutePropertiesContent();

private:
    PlannedWaypointCustomEditingContent plannedContent_;
    DownloadedWaypointCustomEditingContentCreator downloadedContentCreator_;
    CustomWaypointUploadContentCreator routeUploadContentCreator_;
};
