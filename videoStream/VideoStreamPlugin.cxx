/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#include <VideoStreamPlugin.h>

#include <LmCdl/I_InteractableVideoDisplayApi.h>
#include <LmCdl/I_VideoStreamApiCollection.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>
#include <VideoStreamContent.h>

VideoStreamPlugin::VideoStreamPlugin()
    : videoStreamApiCollection_(nullptr)
    , interactableVideoDisplayApi_(nullptr)
{
}

VideoStreamPlugin::~VideoStreamPlugin() { }

QList<LmCdl::PluginRequirement> VideoStreamPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VIDEO_STREAM_API_COLLECTION_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(INTERACTABLE_VIDEO_DISPLAY_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier VideoStreamPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool VideoStreamPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VIDEO_STREAM_API_COLLECTION_CAPABILITY_NAME) {
        videoStreamApiCollection_ = dynamic_cast<LmCdl::I_VideoStreamApiCollection*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == INTERACTABLE_VIDEO_DISPLAY_API_CAPABILITY_NAME) {
        interactableVideoDisplayApi_ = dynamic_cast<LmCdl::I_InteractableVideoDisplayApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* VideoStreamPlugin::getProvidedApi() { return nullptr; }

bool VideoStreamPlugin::isFullyInitialized() const
{
    return (videoStreamApiCollection_ && interactableVideoDisplayApi_);
}

void VideoStreamPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new VideoStreamContent(*videoStreamApiCollection_, *interactableVideoDisplayApi_));
    }
}
