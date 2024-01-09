/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <VectorDataExamplePlugin.h>

#include <LmCdl/I_AggregateVectorDataLoader.h>
#include <LmCdl/I_CenterMapOnLocationApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiStandardPaths.h>
#include <LmCdl/I_VectorDataLayerRegistry.h>
#include <LmCdl/I_VectorDataLoaderRegistry.h>
#include <LmCdl/I_VectorDataWriterRegistry.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>
#include <VectorDataExampleContent.h>

VectorDataExamplePlugin::VectorDataExamplePlugin()
    : loaderRegistry_(nullptr)
    , layerRegistry_(nullptr)
    , vectorDataWriters_(nullptr)
    , mainDataLoader_(nullptr)
    , centerMapApi_(nullptr)
    , applicationApi_(nullptr)
    , standardPathsApi_(nullptr)
{
}

VectorDataExamplePlugin::~VectorDataExamplePlugin() { }

QList<LmCdl::PluginRequirement> VectorDataExamplePlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VECTOR_DATA_LOADER_REGISTRY_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VECTOR_DATA_LAYER_REGISTRY_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VECTOR_DATA_WRITER_REGISTRY_CAPABILITY_NAME, VECTOR_DATA_WRITER_REGISTRY_VERSION_1),
        LmCdl::PluginRequirement(AGGREGATE_VECTOR_DATA_LOADER_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(CENTER_MAP_ON_LOCATION_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_STANDARD_PATHS_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier VectorDataExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool VectorDataExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VECTOR_DATA_LOADER_REGISTRY_CAPABILITY_NAME) {
        loaderRegistry_ = dynamic_cast<LmCdl::I_VectorDataLoaderRegistry*>(api);
        capabilityFound = true;
    } else if (id.capabilityName() == VECTOR_DATA_LAYER_REGISTRY_CAPABILITY_NAME) {
        layerRegistry_ = dynamic_cast<LmCdl::I_VectorDataLayerRegistry*>(api);
        capabilityFound = true;
    } else if (id.capabilityName() == VECTOR_DATA_WRITER_REGISTRY_CAPABILITY_NAME) {
        vectorDataWriters_ = dynamic_cast<LmCdl::I_VectorDataWriterRegistry*>(api);
        capabilityFound = true;
    } else if (id.capabilityName() == AGGREGATE_VECTOR_DATA_LOADER_CAPABILITY_NAME) {
        mainDataLoader_ = dynamic_cast<LmCdl::I_AggregateVectorDataLoader*>(api);
        capabilityFound = true;
    } else if (id.capabilityName() == CENTER_MAP_ON_LOCATION_CAPABILITY_NAME) {
        centerMapApi_ = dynamic_cast<LmCdl::I_CenterMapOnLocationApi*>(api);
        capabilityFound = true;
    } else if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    } else if (id.capabilityName() == VCSI_STANDARD_PATHS_CAPABILITY_NAME) {
        standardPathsApi_ = dynamic_cast<LmCdl::I_VcsiStandardPaths*>(api);
        capabilityFound = true;
    }
    startPluginIfInitialized();
    return capabilityFound;
}

QObject* VectorDataExamplePlugin::getProvidedApi() { return nullptr; }

bool VectorDataExamplePlugin::isFullyInitialized() const
{
    return loaderRegistry_ && layerRegistry_ && vectorDataWriters_ && mainDataLoader_ && centerMapApi_
        && applicationApi_ && standardPathsApi_;
}

void VectorDataExamplePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        loaderRegistry_->registerVectorDataLoader(
            std::unique_ptr<VectorDataExampleContent>(new VectorDataExampleContent(*layerRegistry_, *vectorDataWriters_,
                *mainDataLoader_, *centerMapApi_, *applicationApi_, *standardPathsApi_)));
    }
}
