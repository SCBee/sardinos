/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <ExampleNotificationContent.h>

#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/UniqueIdentifier.h>

ExampleNotificationContent::ExampleNotificationContent(
    LmCdl::UniqueIdentifier vehicleId, LmCdl::I_Vehicle& vehicle, LmCdl::I_VcsiUserNotificationApi& userNotificationApi)
    : longStringNotification_(vehicleId, vehicle, userNotificationApi)
    , notificationToBeRemoved_(vehicleId, userNotificationApi)
{
}

ExampleNotificationContent::~ExampleNotificationContent() { }
