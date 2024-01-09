/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <ExamplePayloadDrawingApiPlugin.h>

#include <ExamplePayloadDrawingApiPluginContent.h>
#include <LmCdl/I_GroundElevationApi.h>
#include <LmCdl/I_PayloadDrawingApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>

ExamplePayloadDrawingApiPlugin::ExamplePayloadDrawingApiPlugin() { }

ExamplePayloadDrawingApiPlugin::~ExamplePayloadDrawingApiPlugin() { }

QList<LmCdl::PluginRequirement> ExamplePayloadDrawingApiPlugin::requiredApis() const
{
    return {
        LmCdl::PluginRequirement(PAYLOAD_DRAWING_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(GROUND_ELEVATION_API_CAPABILITY_NAME, 1, 0, 0),
    };
}

LmCdl::PluginCapabilityIdentifier ExamplePayloadDrawingApiPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier();
}

bool ExamplePayloadDrawingApiPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    if (id.capabilityName() == PAYLOAD_DRAWING_API_CAPABILITY_NAME) {
        auto payloadDrawingApi = dynamic_cast<LmCdl::I_PayloadDrawingApi*>(api);
        if (!payloadDrawingApi) {
            return false;
        }
        payloadDrawingApi_ = payloadDrawingApi;
    } else if (id.capabilityName() == GROUND_ELEVATION_API_CAPABILITY_NAME) {
        auto groundElevationApi = dynamic_cast<LmCdl::I_GroundElevationApi*>(api);
        if (!groundElevationApi) {
            return false;
        }
        groundElevationApi_ = groundElevationApi;
    } else {
        return false;
    }

    if (allRequirementsProvided()) {
        createPluginContent();
    }
    return true;
}

QObject* ExamplePayloadDrawingApiPlugin::getProvidedApi() { return nullptr; }

bool ExamplePayloadDrawingApiPlugin::isFullyInitialized() const { return content_; }

bool ExamplePayloadDrawingApiPlugin::allRequirementsProvided() const
{
    return payloadDrawingApi_ && groundElevationApi_;
}

void ExamplePayloadDrawingApiPlugin::createPluginContent()
{
    content_.reset(new ExamplePayloadDrawingApiPluginContent(*payloadDrawingApi_, *groundElevationApi_));
}
