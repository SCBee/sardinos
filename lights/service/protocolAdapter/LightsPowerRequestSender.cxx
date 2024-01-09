/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsPowerRequestSender.h>

#include <I_LightsStateModel.h>
#include <I_SingleLightModel.h>
#include <LmCdl/I_StanagE2MessagePolicy.h>
#include <LmCdl/I_StanagE2MessageSender.h>
#include <LmCdl/I_StanagE2Messaging.h>
#include <LmCdl/I_Vehicle.h>
#include <Message44.h>

LightsPowerRequestSender::LightsPowerRequestSender(
    LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId, I_LightsStateModel& lightsStateModel)
    : vehicleId_(vehicleId)
    , messageSender_(vehicle.messaging().messageSender())
    , lightsStateModel_(lightsStateModel)
{
    connect(dynamic_cast<const QObject*>(&lightsStateModel_), SIGNAL(anyLightCommandStateChanged()), this,
        SLOT(updateLightCommandState()));
}

LightsPowerRequestSender::~LightsPowerRequestSender() { }

void LightsPowerRequestSender::updateLightCommandState()
{
    LmCdl::Message44 message;
    message.enabledLightsBitmap = combinedLightPowerStateDemand();
    messageSender_.send(message);
}

int LightsPowerRequestSender::combinedLightPowerStateDemand()
{
    int combinedValue = 0;
    combinedValue |= encodedLightBit(0, lightsStateModel_.lightModel(StanagLightType::Nav).isCommandedOn());
    combinedValue |= encodedLightBit(1, lightsStateModel_.lightModel(StanagLightType::NavIr).isCommandedOn());
    combinedValue |= encodedLightBit(2, lightsStateModel_.lightModel(StanagLightType::Strobe).isCommandedOn());
    combinedValue |= encodedLightBit(3, lightsStateModel_.lightModel(StanagLightType::StrobeIr).isCommandedOn());
    combinedValue |= encodedLightBit(4, lightsStateModel_.lightModel(StanagLightType::Nvd).isCommandedOn());
    combinedValue |= encodedLightBit(5, lightsStateModel_.lightModel(StanagLightType::Reserved1).isCommandedOn());
    combinedValue |= encodedLightBit(6, lightsStateModel_.lightModel(StanagLightType::Landing).isCommandedOn());
    combinedValue |= encodedLightBit(7, lightsStateModel_.lightModel(StanagLightType::LandingIr).isCommandedOn());
    combinedValue
        |= encodedLightBit(8, lightsStateModel_.lightModel(StanagLightType::VehicleSpecific1).isCommandedOn());
    combinedValue
        |= encodedLightBit(9, lightsStateModel_.lightModel(StanagLightType::VehicleSpecific2).isCommandedOn());
    combinedValue
        |= encodedLightBit(10, lightsStateModel_.lightModel(StanagLightType::VehicleSpecific3).isCommandedOn());
    combinedValue
        |= encodedLightBit(11, lightsStateModel_.lightModel(StanagLightType::VehicleSpecific4).isCommandedOn());
    combinedValue
        |= encodedLightBit(12, lightsStateModel_.lightModel(StanagLightType::VehicleSpecific5).isCommandedOn());
    combinedValue
        |= encodedLightBit(13, lightsStateModel_.lightModel(StanagLightType::VehicleSpecific6).isCommandedOn());
    combinedValue
        |= encodedLightBit(14, lightsStateModel_.lightModel(StanagLightType::VehicleSpecific7).isCommandedOn());
    combinedValue
        |= encodedLightBit(15, lightsStateModel_.lightModel(StanagLightType::VehicleSpecific8).isCommandedOn());
    return combinedValue;
}

int LightsPowerRequestSender::encodedLightBit(int offset, bool isOn)
{
    int value = isOn ? 1 : 0;
    return value << offset;
}
