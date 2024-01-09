/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <StanagE2PrivateMessageContentCreator.h>

#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <StanagE2PrivateMessageContent.h>

StanagE2PrivateMessageContentCreator::StanagE2PrivateMessageContentCreator(
    LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_VcsiVehicleExtensionApi& vehicleWidgetApi)
    : vehicleApi_(vehicleApi)
    , vehicleWidgetApi_(vehicleWidgetApi)
{
    connect(&vehicleApi, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this,
        &StanagE2PrivateMessageContentCreator::makeDeflectorControlForVehicle);
    connect(&vehicleApi, &LmCdl::I_VehicleCollectionApi::vehicleAboutToBeRemoved, this,
        &StanagE2PrivateMessageContentCreator::removeDeflectorControlForVehicle);
}

StanagE2PrivateMessageContentCreator::~StanagE2PrivateMessageContentCreator() = default;

void StanagE2PrivateMessageContentCreator::makeDeflectorControlForVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    LmCdl::I_Vehicle& vehicle = vehicleApi_.vehicle(vehicleId);
    deflectorControls_.emplace(vehicleId,
        std::unique_ptr<StanagE2PrivateMessageContent>(
            new StanagE2PrivateMessageContent(vehicleId, vehicleWidgetApi_, vehicle)));
}

void StanagE2PrivateMessageContentCreator::removeDeflectorControlForVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    deflectorControls_.erase(vehicleId);
}
