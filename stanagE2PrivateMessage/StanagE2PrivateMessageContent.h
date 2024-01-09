#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/UniqueIdentifier.h>
#include <StanagE2PrivateMessageControlWidgets.h>
#include <StanagE2PrivateMessagePolarityReceiver.h>
#include <StanagE2PrivateMessagePolaritySender.h>

namespace LmCdl {
class I_VcsiVehicleExtensionApi;
class I_StanagE2Messaging;
}

class StanagE2PrivateMessageContent {
public:
    StanagE2PrivateMessageContent(
        LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiVehicleExtensionApi& widgetApi, LmCdl::I_Vehicle& vehicle);

private:
    StanagE2PrivateMessageControlWidgets widgets_;
    StanagE2PrivateMessagePolaritySender polaritySender_;
    StanagE2PrivateMessagePolarityReceiver polarityReceiver_;
};
