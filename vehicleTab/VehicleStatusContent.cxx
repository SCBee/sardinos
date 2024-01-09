/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <VehicleStatusContent.h>

#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <SingleVehicleStatusDisplay.h>

VehicleStatusContent::VehicleStatusContent(LmCdl::I_VehicleCollectionApi& vehicleCollectionApi,
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::I_UnitPreferencesApi& unitPreferencesApi)
    : vehicleCollectionApi_(vehicleCollectionApi)
    , vehicleExtensionApi_(vehicleExtensionApi)
    , unitPreferencesApi_(unitPreferencesApi)
{
    connect(&vehicleCollectionApi_, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this,
        &VehicleStatusContent::addNewVehicle);
    connect(&vehicleCollectionApi_, &LmCdl::I_VehicleCollectionApi::vehicleAboutToBeRemoved, this,
        &VehicleStatusContent::removeVehicle);
}

VehicleStatusContent::~VehicleStatusContent() = default;

void VehicleStatusContent::addNewVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    LmCdl::I_Vehicle& vehicle = vehicleCollectionApi_.vehicle(vehicleId);
    vehicleDisplays_.emplace(vehicleId,
        std::unique_ptr<SingleVehicleStatusDisplay>(
            new SingleVehicleStatusDisplay(vehicle, vehicleExtensionApi_, vehicleId, unitPreferencesApi_)));
}

void VehicleStatusContent::removeVehicle(LmCdl::UniqueIdentifier vehicleId) { vehicleDisplays_.erase(vehicleId); }
