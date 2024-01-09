/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <CustomCloakingDeviceUploadSource.h>

#include <CloakingDeviceJsonDefines.h>
#include <LmCdl/I_StanagE2MessageSender.h>
#include <LmCdl/StanagWaypoint.h>
#include <Message30265.h>

CustomCloakingDeviceUploadSource::CustomCloakingDeviceUploadSource() { }

CustomCloakingDeviceUploadSource::~CustomCloakingDeviceUploadSource() { }

void CustomCloakingDeviceUploadSource::uploadMessages(
    const LmCdl::StanagWaypoint& waypoint, LmCdl::I_StanagE2MessageSender& sender)
{
    auto powerValue = cloakingDevicePowerValue(waypoint.customPropertyExtension);
    auto waypointId = waypoint.waypointId;

    if (isPowerValueValid(powerValue)) {
        sendCloakingDevicePowerMessage(waypointId, sender, powerValueToBool(powerValue));
    }
}

QString CustomCloakingDeviceUploadSource::cloakingDevicePowerValue(const QJsonObject& json)
{
    auto cloakingDeviceProperties = json.value(CloakingDeviceJsonDefines::CLOAKING_DEVICE_TAG).toObject();
    return cloakingDeviceProperties.value(CloakingDeviceJsonDefines::POWER_TAG).toString();
}

bool CustomCloakingDeviceUploadSource::powerValueToBool(const QString& powerValue)
{
    if (CloakingDeviceJsonDefines::ON_VALUE == powerValue) {
        return true;
    }
    return false;
}

bool CustomCloakingDeviceUploadSource::isPowerValueValid(const QString& powerValue)
{
    return (
        (powerValue == CloakingDeviceJsonDefines::ON_VALUE) || (powerValue == CloakingDeviceJsonDefines::OFF_VALUE));
}

void CustomCloakingDeviceUploadSource::sendCloakingDevicePowerMessage(
    LmCdl::UniqueIdentifier waypointId, LmCdl::I_StanagE2MessageSender& sender, bool turnOnCloakingDevice)
{
    LmCdl::Message30265 message;
    message.waypointNumber = waypointId.value();

    message.cloakingDevicePower = turnOnCloakingDevice ? LmCdl::Message30265::POWER_ON : LmCdl::Message30265::POWER_OFF;
    sender.send(message);
}
