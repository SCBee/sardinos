/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <RestrictionZoneApiExamplePlugin.h>

#include <LmCdl/I_AggregateVectorDataLoader.h>
#include <LmCdl/I_KeepOutRestrictionZoneCollection.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VectorDataLayerRegistry.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>
#include <RestrictionZoneApiExampleContent.h>
#include <RestrictionZoneApiExampleView.h>

RestrictionZoneApiExamplePlugin::RestrictionZoneApiExamplePlugin()
    : keepOutRestrictionZoneCollectionApi_(nullptr)
    , layerRegistry_(nullptr)
    , mainDataLoader_(nullptr)
    , applicationApi_(nullptr)
{
}

RestrictionZoneApiExamplePlugin::~RestrictionZoneApiExamplePlugin() { }

QList<LmCdl::PluginRequirement> RestrictionZoneApiExamplePlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(KEEP_OUT_RESTRICTION_ZONE_COLLECTION_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VECTOR_DATA_LAYER_REGISTRY_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(AGGREGATE_VECTOR_DATA_LOADER_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier RestrictionZoneApiExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool RestrictionZoneApiExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == KEEP_OUT_RESTRICTION_ZONE_COLLECTION_CAPABILITY_NAME) {
        keepOutRestrictionZoneCollectionApi_ = dynamic_cast<LmCdl::I_KeepOutRestrictionZoneCollection*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VECTOR_DATA_LAYER_REGISTRY_CAPABILITY_NAME) {
        layerRegistry_ = dynamic_cast<LmCdl::I_VectorDataLayerRegistry*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == AGGREGATE_VECTOR_DATA_LOADER_CAPABILITY_NAME) {
        mainDataLoader_ = dynamic_cast<LmCdl::I_AggregateVectorDataLoader*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* RestrictionZoneApiExamplePlugin::getProvidedApi() { return nullptr; }

bool RestrictionZoneApiExamplePlugin::isFullyInitialized() const
{
    return (keepOutRestrictionZoneCollectionApi_ && layerRegistry_ && mainDataLoader_ && applicationApi_);
}

void RestrictionZoneApiExamplePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        restrictionZoneApiExampleContent_.reset(new RestrictionZoneApiExampleContent(
            *keepOutRestrictionZoneCollectionApi_, *layerRegistry_, *mainDataLoader_));
        restrictionZoneApiExampleView_.reset(
            new RestrictionZoneApiExampleView(*applicationApi_, *restrictionZoneApiExampleContent_));
    }
}
