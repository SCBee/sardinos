/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsStatusReceiver.h>

#include <I_LightsStateModel.h>
#include <LmCdl/I_StanagE2MessageReceiver.h>
#include <LmCdl/I_StanagE2Messaging.h>
#include <LmCdl/I_Vehicle.h>
#include <Message107.h>

LightsStatusReceiver::LightsStatusReceiver(
    LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId, I_LightsStateModel& model)
    : vehicleId_(vehicleId)
    , lightsStateModel_(model)
{
    connect(&vehicle.messaging().allCucsMessageReceiver(), // Will process any
                                                           // message for the
                                                           // vehicle, regardless
                                                           // of the CUCS ID
        &LmCdl::I_StanagE2MessageReceiver::messageReceived, this, &LightsStatusReceiver::processMessageData);
}

LightsStatusReceiver::~LightsStatusReceiver() { }

void LightsStatusReceiver::processMessageData(const LmCdl::StanagMessageType& type, const QByteArray& contents)
{
    if (type == LmCdl::Message107::staticMessageType()) {
        LmCdl::Message107 message;
        message.parse(contents);
        int encodedLightsField = message.enabledLightsBitmap;
        lightsStateModel_.editableLightModel(StanagLightType::Nav)
            .setReportedState(isLightAtOffsetOn(0, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::NavIr)
            .setReportedState(isLightAtOffsetOn(1, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::Strobe)
            .setReportedState(isLightAtOffsetOn(2, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::StrobeIr)
            .setReportedState(isLightAtOffsetOn(3, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::Nvd)
            .setReportedState(isLightAtOffsetOn(4, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::Reserved1)
            .setReportedState(isLightAtOffsetOn(5, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::Landing)
            .setReportedState(isLightAtOffsetOn(6, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::LandingIr)
            .setReportedState(isLightAtOffsetOn(7, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::VehicleSpecific1)
            .setReportedState(isLightAtOffsetOn(8, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::VehicleSpecific2)
            .setReportedState(isLightAtOffsetOn(9, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::VehicleSpecific3)
            .setReportedState(isLightAtOffsetOn(10, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::VehicleSpecific4)
            .setReportedState(isLightAtOffsetOn(11, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::VehicleSpecific5)
            .setReportedState(isLightAtOffsetOn(12, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::VehicleSpecific6)
            .setReportedState(isLightAtOffsetOn(13, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::VehicleSpecific7)
            .setReportedState(isLightAtOffsetOn(14, encodedLightsField));
        lightsStateModel_.editableLightModel(StanagLightType::VehicleSpecific8)
            .setReportedState(isLightAtOffsetOn(15, encodedLightsField));
    }
}

bool LightsStatusReceiver::isLightAtOffsetOn(int offset, int encodedField) const
{
    int isLightOn = (encodedField >> offset) & 0x00000001;
    qDebug("isLightAtOffsetOn: %d, 0x%x - %d", offset, encodedField, isLightOn);
    return (isLightOn);
}
