/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <memory>

namespace LmCdl {
class I_RouteApi;
class I_VehicleCollectionApi;
}

class CustomWaypointUploadContent;

class CustomWaypointUploadContentCreator : public QObject {
public:
    explicit CustomWaypointUploadContentCreator(LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi);
    virtual ~CustomWaypointUploadContentCreator();

private:
    void makeRouteUploadContentForVehicle(LmCdl::UniqueIdentifier vehicleId);

private:
    LmCdl::I_VehicleCollectionApi& vehicleApi_;
    LmCdl::I_RouteApi& routeApi_;

    std::vector<std::unique_ptr<CustomWaypointUploadContent>> routeUploadContent_;
};
