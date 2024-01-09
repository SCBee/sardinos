/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsConfigurationReceiver.h>

#include <I_LightsStateModel.h>
#include <LmCdl/I_StanagE2MessageReceiver.h>
#include <LmCdl/I_StanagE2Messaging.h>
#include <LmCdl/I_Vehicle.h>
#include <Message1302.h>
#include <Message1303.h>
#include <QtGlobal>

namespace {
bool isEnumSupported(LmCdl::Message1303 message)
{
    if (LmCdl::Message1303::NotImplemented == (qint8)message.enumerationAvailability) {
        qDebug("supported false (%d)", (int)message.enumerationAvailability);
        return false;
    }
    qDebug("supported true (%d)(%d)", (int)LmCdl::Message1303::NotImplemented, (qint8)message.enumerationAvailability);
    return true;
}
bool isEnumAvailable(LmCdl::Message1303 message)
{
    if ((int)LmCdl::Message1303::Enabled == (int)message.enumerationAvailability) {
        qDebug("available true (%d)", (int)message.enumerationAvailability);
        return true;
    }
    qDebug("available true (%d)", (int)message.enumerationAvailability);
    return false;
}
}

LightsConfigurationReceiver::LightsConfigurationReceiver(
    LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId, I_LightsStateModel& model)
    : vehicleId_(vehicleId)
    , lightsStateModel_(model)
{
    connect(&vehicle.messaging().localCucsMessageReceiver(), // Will process any
                                                             // message for the
                                                             // vehicle,
                                                             // regardless of the
                                                             // CUCS ID
        &LmCdl::I_StanagE2MessageReceiver::messageReceived, this, &LightsConfigurationReceiver::processMessageData);
}

LightsConfigurationReceiver::~LightsConfigurationReceiver() { }

void LightsConfigurationReceiver::processMessageData(
    const LmCdl::StanagMessageType& msgType, const QByteArray& contents)
{
    if (msgType == LmCdl::Message1302::staticMessageType()) {
        LmCdl::Message1302 message;
        message.parse(contents);
        if (message.messageNumber == 44 && message.fieldNumber == 4) {
            StanagLightType lightType = typeFromMessage(message.enumerationIndex);
            lightsStateModel_.editableLightModel(lightType).setLightName(message.enumerationText);
        }
    } else if (msgType == LmCdl::Message1303::staticMessageType()) {
        LmCdl::Message1303 message;
        message.parse(contents);

        if (message.messageNumber == 44 && message.fieldNumber == 4) {
            StanagLightType lightType = typeFromMessage(message.enumerationIndex);
            lightsStateModel_.editableLightModel(lightType).setSupported(isEnumSupported(message));
            lightsStateModel_.editableLightModel(lightType).setAvailability(isEnumAvailable(message));
        }
    }
}

StanagLightType LightsConfigurationReceiver::typeFromMessage(int messageEnumerationIndex) const
{
    switch (messageEnumerationIndex) {
    case 0x01:
        return StanagLightType::Nav;
    case 0x02:
        return StanagLightType::NavIr;

    default:
        return StanagLightType::Reserved1;
    }
}
