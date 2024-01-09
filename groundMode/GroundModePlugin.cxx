/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <GroundModePlugin.h>

#include <GroundModeContentCreator.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

GroundModePlugin::GroundModePlugin()
    : vehicleCollectionApi_(nullptr)
{
}

GroundModePlugin::~GroundModePlugin() { }

QList<LmCdl::PluginRequirement> GroundModePlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VEHICLE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier GroundModePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool GroundModePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VEHICLE_COLLECTION_API_CAPABILITY_NAME) {
        vehicleCollectionApi_ = dynamic_cast<LmCdl::I_VehicleCollectionApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* GroundModePlugin::getProvidedApi() { return nullptr; }

bool GroundModePlugin::isFullyInitialized() const { return (vehicleCollectionApi_); }

void GroundModePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        contentCreator_.reset(new GroundModeContentCreator(*vehicleCollectionApi_));
    }
}
