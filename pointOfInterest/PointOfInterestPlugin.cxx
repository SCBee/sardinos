/**
 \file
 Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
 */

#include <PointOfInterestPlugin.h>

#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <PointOfInterestContent.h>
#include <QList>

PointOfInterestPlugin::PointOfInterestPlugin()
    : pointOfInterestApi_(nullptr)
{
}

PointOfInterestPlugin::~PointOfInterestPlugin() { }

QList<LmCdl::PluginRequirement> PointOfInterestPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(POINT_OF_INTEREST_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier PointOfInterestPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool PointOfInterestPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == POINT_OF_INTEREST_API_CAPABILITY_NAME) {
        pointOfInterestApi_ = dynamic_cast<LmCdl::I_PointOfInterestApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* PointOfInterestPlugin::getProvidedApi() { return nullptr; }

bool PointOfInterestPlugin::isFullyInitialized() const { return (pointOfInterestApi_); }

void PointOfInterestPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new PointOfInterestContent(*pointOfInterestApi_));
    }
}
