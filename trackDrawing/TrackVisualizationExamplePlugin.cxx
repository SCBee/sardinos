/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <TrackVisualizationExamplePlugin.h>

#include <LmCdl/I_TrackDrawingApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <TrackVisualizationExamplePluginContent.h>

TrackVisualizationExamplePlugin::TrackVisualizationExamplePlugin()
    : trackDrawingApi_(nullptr)
{
}

TrackVisualizationExamplePlugin::~TrackVisualizationExamplePlugin() { }

QList<LmCdl::PluginRequirement> TrackVisualizationExamplePlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(TRACK_DRAWING_API_CAPABILITY_NAME, TRACK_DRAWING_API_VERSION_1) };
}

LmCdl::PluginCapabilityIdentifier TrackVisualizationExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier();
}

bool TrackVisualizationExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    if (id.capabilityName() == TRACK_DRAWING_API_CAPABILITY_NAME) {
        auto trackDrawingApi = dynamic_cast<LmCdl::I_TrackDrawingApi*>(api);
        if (!trackDrawingApi) {
            return false;
        }
        trackDrawingApi_ = trackDrawingApi;
    } else {
        return false;
    }

    if (allRequirementsProvided()) {
        createPluginContent();
    }
    return true;
}

QObject* TrackVisualizationExamplePlugin::getProvidedApi() { return nullptr; }

bool TrackVisualizationExamplePlugin::isFullyInitialized() const { return content_; }

bool TrackVisualizationExamplePlugin::allRequirementsProvided() const { return trackDrawingApi_; }

void TrackVisualizationExamplePlugin::createPluginContent()
{
    content_.reset(new TrackVisualizationExamplePluginContent(*trackDrawingApi_));
}
