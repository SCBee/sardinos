/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <StanagE2PrivateMessagePlugin.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>
#include <StanagE2PrivateMessageContentCreator.h>

StanagE2PrivateMessagePlugin::StanagE2PrivateMessagePlugin()
    : vehicleApi_(nullptr)
    , applicationApi_(nullptr)
{
}

StanagE2PrivateMessagePlugin::~StanagE2PrivateMessagePlugin() { }

QList<LmCdl::PluginRequirement> StanagE2PrivateMessagePlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VEHICLE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier StanagE2PrivateMessagePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool StanagE2PrivateMessagePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VEHICLE_COLLECTION_API_CAPABILITY_NAME) {
        vehicleApi_ = dynamic_cast<LmCdl::I_VehicleCollectionApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* StanagE2PrivateMessagePlugin::getProvidedApi() { return nullptr; }

bool StanagE2PrivateMessagePlugin::isFullyInitialized() const { return (vehicleApi_ && applicationApi_); }

void StanagE2PrivateMessagePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        vehicleContentCreator_.reset(
            new StanagE2PrivateMessageContentCreator(*vehicleApi_, applicationApi_->widgetExtensionApi().vehicleApi()));
    }
}
