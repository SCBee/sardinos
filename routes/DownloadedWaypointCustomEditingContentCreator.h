#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <memory>
#include <vector>
class DownloadedWaypointCustomEditingContent;

namespace LmCdl {
class I_VehicleCollectionApi;
class I_RouteApi;
}

class DownloadedWaypointCustomEditingContentCreator : public QObject {
public:
    DownloadedWaypointCustomEditingContentCreator(
        LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi);
    ~DownloadedWaypointCustomEditingContentCreator();

private:
    void makeDownloadedRouteContentForVehicle(LmCdl::UniqueIdentifier vehicleId);

private:
    LmCdl::I_VehicleCollectionApi& vehicleApi_;
    LmCdl::I_RouteApi& routeApi_;

    std::vector<std::unique_ptr<DownloadedWaypointCustomEditingContent>> downloadedRouteContent_;
};
