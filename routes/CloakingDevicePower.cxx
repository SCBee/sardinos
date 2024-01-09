/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <CloakingDevicePower.h>

#include <CloakingDeviceJsonDefines.h>

CloakingDevicePower::CloakingDevicePower()
    : cloakingDevicePower_()
{
}

CloakingDevicePower::~CloakingDevicePower() { }

void CloakingDevicePower::setCloakingDeviceOn(LmCdl::UniqueIdentifier waypoint, bool isOn)
{
    if (!cloakingDevicePower_.contains(waypoint) || isCloakingDeviceOn(waypoint) != isOn) {
        cloakingDevicePower_.insert(waypoint, isOn);
        emit cloakingDevicePowerChanged(waypoint);
    }
}

QString CloakingDevicePower::cloakingDevicePowerToString(LmCdl::UniqueIdentifier waypoint) const
{
    return isCloakingDeviceOn(waypoint) ? CloakingDeviceJsonDefines::ON_VALUE : CloakingDeviceJsonDefines::OFF_VALUE;
}

bool CloakingDevicePower::isCloakingDeviceOn(LmCdl::UniqueIdentifier waypoint) const
{
    return cloakingDevicePower_.value(waypoint, false);
}
