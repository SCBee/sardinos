/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <AnimalTrackerPlugin.h>

#include <AnimalTrackerContent.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

AnimalTrackerPlugin::AnimalTrackerPlugin()
    : pointOfInterestApi_(nullptr)
{
}

AnimalTrackerPlugin::~AnimalTrackerPlugin() { }

QList<LmCdl::PluginRequirement> AnimalTrackerPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(POINT_OF_INTEREST_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier AnimalTrackerPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier("GdWild::I_AnimalTrackerApi", 1, 0, 0);
}

bool AnimalTrackerPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == POINT_OF_INTEREST_API_CAPABILITY_NAME) {
        pointOfInterestApi_ = dynamic_cast<LmCdl::I_PointOfInterestApi*>(api);
        capabilityFound = true;
    }
    startPluginIfInitialized();
    return capabilityFound;
}

QObject* AnimalTrackerPlugin::getProvidedApi()
{
    if (isFullyInitialized()) {
        return pluginContent_.data();
    }
    return nullptr;
}

bool AnimalTrackerPlugin::isFullyInitialized() const { return (pointOfInterestApi_); }

void AnimalTrackerPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new AnimalTrackerContent(*pointOfInterestApi_));
    }
}
