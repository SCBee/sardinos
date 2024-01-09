/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <DownloadedCloakingDevicePowerMessageReceiver.h>

#include <CloakingDevicePower.h>
#include <LmCdl/I_StanagE2MessageReceiver.h>
#include <LmCdl/I_StanagE2Messaging.h>
#include <LmCdl/I_Vehicle.h>
#include <Message33265.h>

DownloadedCloakingDevicePowerMessageReceiver::DownloadedCloakingDevicePowerMessageReceiver(
    LmCdl::I_Vehicle& vehicle, CloakingDevicePower& cloakingDevicePower)
    : cloakingDevicePower_(cloakingDevicePower)
{
    connect(&vehicle.messaging().localCucsMessageReceiver(), &LmCdl::I_StanagE2MessageReceiver::messageReceived, this,
        &DownloadedCloakingDevicePowerMessageReceiver::processReceivedData);
}

DownloadedCloakingDevicePowerMessageReceiver::~DownloadedCloakingDevicePowerMessageReceiver() { }

void DownloadedCloakingDevicePowerMessageReceiver::processReceivedData(
    const LmCdl::StanagMessageType& type, const QByteArray& contents)
{
    if (type == LmCdl::Message33265::staticMessageType()) {
        LmCdl::Message33265 message;
        message.parse(contents);
        setCloakingDevicePower(message);
    }
}

void DownloadedCloakingDevicePowerMessageReceiver::setCloakingDevicePower(const LmCdl::Message33265& message)
{
    const LmCdl::UniqueIdentifier waypointId(message.waypointNumber);
    switch (message.cloakingDevicePower) {
    case LmCdl::Message33265::CloakingDevicePower::POWER_OFF:
        cloakingDevicePower_.setCloakingDeviceOn(waypointId, false);
        break;
    case LmCdl::Message33265::CloakingDevicePower::POWER_ON:
        cloakingDevicePower_.setCloakingDeviceOn(waypointId, true);
        break;
    }
}
