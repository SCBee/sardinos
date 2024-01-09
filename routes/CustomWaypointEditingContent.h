/**
\file
Copyright (c) 2017 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <QtGlobal>

#include <DownloadedWaypointCustomEditingContentCreator.h>
#include <PlannedWaypointCustomEditingContent.h>

class CustomWaypointEditingContent {
public:
    explicit CustomWaypointEditingContent(LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi);
    virtual ~CustomWaypointEditingContent();

private:
    Q_DISABLE_COPY(CustomWaypointEditingContent)

    PlannedWaypointCustomEditingContent plannedContent_;
    DownloadedWaypointCustomEditingContentCreator downloadedContentCreator_;
};
