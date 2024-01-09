/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <ReadonlyRoutesContentCreator.h>

#include <LmCdl/I_VehicleCollectionApi.h>
#include <ReadonlyRoutesContent.h>

ReadonlyRoutesContentCreator::ReadonlyRoutesContentCreator(LmCdl::I_VehicleCollectionApi& vehicleApi,
    LmCdl::I_RouteApi& routeApi, LmCdl::I_VcsiVehicleExtensionApi& vehicleWidgetApi)
    : vehicleApi_(vehicleApi)
    , routeApi_(routeApi)
    , vehicleWidgetApi_(vehicleWidgetApi)
{
    connect(&vehicleApi, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this,
        &ReadonlyRoutesContentCreator::makeVehicleContent);
}

ReadonlyRoutesContentCreator::~ReadonlyRoutesContentCreator() = default;

void ReadonlyRoutesContentCreator::makeVehicleContent(LmCdl::UniqueIdentifier vehicleId)
{
    downloadedRoutes_.emplace_back(new ReadonlyRoutesContent(vehicleId, vehicleWidgetApi_, routeApi_));
}
