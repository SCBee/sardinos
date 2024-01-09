/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <CustomWaypointUploadContent.h>

#include <LmCdl/I_StanagWaypointUploadSourceRegistry.h>

CustomWaypointUploadContent::CustomWaypointUploadContent(
    LmCdl::I_StanagWaypointUploadSourceRegistry& waypointUploadSourceRegistry)
{
    waypointUploadSourceRegistry.registerUploadSource(cloakingDeviceUploadSource_);
}

CustomWaypointUploadContent::~CustomWaypointUploadContent() { }
