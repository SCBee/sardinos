/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <CustomCloakingDeviceUploadSource.h>

namespace LmCdl {
class I_StanagWaypointUploadSourceRegistry;
}

class CustomWaypointUploadContent {
public:
    explicit CustomWaypointUploadContent(LmCdl::I_StanagWaypointUploadSourceRegistry& waypointUploadSourceRegistry);
    virtual ~CustomWaypointUploadContent();

private:
    CustomCloakingDeviceUploadSource cloakingDeviceUploadSource_;
};
