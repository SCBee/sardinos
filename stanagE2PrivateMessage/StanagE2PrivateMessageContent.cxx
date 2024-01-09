/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <StanagE2PrivateMessageContent.h>

#include <LmCdl/I_StanagE2Messaging.h>

StanagE2PrivateMessageContent::StanagE2PrivateMessageContent(
    LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiVehicleExtensionApi& widgetApi, LmCdl::I_Vehicle& vehicle)
    : widgets_(vehicleId, widgetApi)
    , polaritySender_(widgets_, vehicle.messaging().messageSender(), vehicle.messaging().messagePolicy())
    , polarityReceiver_(vehicle, widgets_)
{
}
