/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_StanagWaypointUploadSource.h>

namespace LmCdl {
class UniqueIdentifier;
}

class QJsonObject;

class CustomCloakingDeviceUploadSource : public LmCdl::I_StanagWaypointUploadSource {
public:
    explicit CustomCloakingDeviceUploadSource();
    virtual ~CustomCloakingDeviceUploadSource();

    void uploadMessages(const LmCdl::StanagWaypoint& waypoint, LmCdl::I_StanagE2MessageSender& sender) override;

private:
    QString cloakingDevicePowerValue(const QJsonObject& json);
    bool powerValueToBool(const QString& powerValue);
    bool isPowerValueValid(const QString& powerValue);
    void sendCloakingDevicePowerMessage(
        LmCdl::UniqueIdentifier waypointId, LmCdl::I_StanagE2MessageSender& sender, bool turnOnCloakingDevice);
};
