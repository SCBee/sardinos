/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <LivestockCounterPlugin.h>

#include <I_AnimalTrackerApi.h>
#include <LivestockCounterContent.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>

namespace {
const char* ANIMAL_TRACKER_API_NAME = "GdWild::I_AnimalTrackerApi";
}
LivestockCounterPlugin::LivestockCounterPlugin()
    : animalTrackerApi_(nullptr)
{
}

LivestockCounterPlugin::~LivestockCounterPlugin() { }

QList<LmCdl::PluginRequirement> LivestockCounterPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(ANIMAL_TRACKER_API_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier LivestockCounterPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier();
}

bool LivestockCounterPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == ANIMAL_TRACKER_API_NAME) {
        animalTrackerApi_ = dynamic_cast<GdWild::I_AnimalTrackerApi*>(api);
        capabilityFound = true;
    }
    startPluginIfInitialized();
    return capabilityFound;
}

QObject* LivestockCounterPlugin::getProvidedApi() { return nullptr; }

bool LivestockCounterPlugin::isFullyInitialized() const { return (animalTrackerApi_); }

void LivestockCounterPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new LivestockCounterContent(*animalTrackerApi_));
    }
}
