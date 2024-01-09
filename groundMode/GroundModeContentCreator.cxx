/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <GroundModeContentCreator.h>

#include <GroundModeContent.h>
#include <LmCdl/I_VehicleCollectionApi.h>

GroundModeContentCreator::GroundModeContentCreator(LmCdl::I_VehicleCollectionApi& api)
    : api_(api)
{
    connect(&api_, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this, &GroundModeContentCreator::addContentForVehicle);
    connect(&api_, &LmCdl::I_VehicleCollectionApi::vehicleAboutToBeRemoved, this,
        &GroundModeContentCreator::removeContentForVehicle);
}

GroundModeContentCreator::~GroundModeContentCreator() { }

void GroundModeContentCreator::addContentForVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    content_.emplace(vehicleId, std::unique_ptr<GroundModeContent>(new GroundModeContent(api_.vehicle(vehicleId))));
}

void GroundModeContentCreator::removeContentForVehicle(LmCdl::UniqueIdentifier vehicleId) { content_.erase(vehicleId); }
