/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <MissionDrawingApiPlugin.h>

#include <LmCdl/I_MissionDrawingApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <MissionDrawingApiPluginContent.h>

MissionDrawingApiPlugin::MissionDrawingApiPlugin()
    : missionDrawingApi_(nullptr)
{
}

MissionDrawingApiPlugin::~MissionDrawingApiPlugin() { }

QList<LmCdl::PluginRequirement> MissionDrawingApiPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(MISSION_DRAWING_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier MissionDrawingApiPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier();
}

bool MissionDrawingApiPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    if (id.capabilityName() == MISSION_DRAWING_API_CAPABILITY_NAME) {
        auto missionDrawingApi = dynamic_cast<LmCdl::I_MissionDrawingApi*>(api);
        if (!missionDrawingApi) {
            return false;
        }
        missionDrawingApi_ = missionDrawingApi;
    } else {
        return false;
    }

    if (allRequirementsProvided()) {
        createPluginContent();
    }
    return true;
}

QObject* MissionDrawingApiPlugin::getProvidedApi() { return nullptr; }

bool MissionDrawingApiPlugin::isFullyInitialized() const { return content_; }

bool MissionDrawingApiPlugin::allRequirementsProvided() const { return missionDrawingApi_; }

void MissionDrawingApiPlugin::createPluginContent()
{
    content_.reset(new MissionDrawingApiPluginContent(*missionDrawingApi_));
}
