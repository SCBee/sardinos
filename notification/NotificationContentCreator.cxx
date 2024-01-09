/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <NotificationContentCreator.h>

#include <ExampleNotificationContent.h>
#include <LmCdl/I_VehicleCollectionApi.h>

NotificationContentCreator::NotificationContentCreator(
    LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_VcsiUserNotificationApi& userNotificationApi)
    : vehicleApi_(vehicleApi)
    , userNotificationApi_(userNotificationApi)
{
    connect(&vehicleApi, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this,
        &NotificationContentCreator::addWarningsForVehicle);
    connect(&vehicleApi, &LmCdl::I_VehicleCollectionApi::vehicleAboutToBeRemoved, this,
        &NotificationContentCreator::removeWarningsForVehicle);
}

NotificationContentCreator::~NotificationContentCreator() { }

void NotificationContentCreator::addWarningsForVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    content_.emplace(vehicleId,
        std::unique_ptr<ExampleNotificationContent>(
            new ExampleNotificationContent(vehicleId, vehicleApi_.vehicle(vehicleId), userNotificationApi_)));
}

void NotificationContentCreator::removeWarningsForVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    content_.erase(vehicleId);
}
