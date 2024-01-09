/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#include <AudioExamplePlugin.h>

#include <AudioExamplePluginContent.h>
#include <LmCdl/I_AudioApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

AudioExamplePlugin::AudioExamplePlugin()
    : audioApi_(nullptr)
    , applicationApi_(nullptr)
{
}

AudioExamplePlugin::~AudioExamplePlugin() { }

QList<LmCdl::PluginRequirement> AudioExamplePlugin::requiredApis() const
{
    QList<LmCdl::PluginRequirement> requirementList;
    requirementList.append(LmCdl::PluginRequirement(AUDIO_API_CAPABILITY_NAME, 1, 0, 0));
    requirementList.append(LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0));
    return requirementList;
}

LmCdl::PluginCapabilityIdentifier AudioExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool AudioExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == AUDIO_API_CAPABILITY_NAME) {
        audioApi_ = dynamic_cast<LmCdl::I_AudioApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* AudioExamplePlugin::getProvidedApi() { return nullptr; }

bool AudioExamplePlugin::isFullyInitialized() const { return (audioApi_ && applicationApi_); }

void AudioExamplePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new AudioExamplePluginContent(*audioApi_, *applicationApi_));
    }
}
