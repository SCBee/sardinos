/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <DynamicVectorDataDrawingPlugin.h>
#include <DynamicVectorDataDrawingPlugin.moc>

#include <DynamicVectorDataDrawingPluginContent.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>

DynamicVectorDataDrawingPlugin::DynamicVectorDataDrawingPlugin() { }

DynamicVectorDataDrawingPlugin::~DynamicVectorDataDrawingPlugin() { }

QList<LmCdl::PluginRequirement> DynamicVectorDataDrawingPlugin::requiredApis() const
{
    return {
        LmCdl::PluginRequirement(VECTOR_DATA_DRAWING_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VEHICLE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0),
    };
}

LmCdl::PluginCapabilityIdentifier DynamicVectorDataDrawingPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier();
}

bool DynamicVectorDataDrawingPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    if (id.capabilityName() == VECTOR_DATA_DRAWING_API_CAPABILITY_NAME) {
        auto vectorDataDrawingApi = dynamic_cast<LmCdl::I_VectorDataDrawingApi*>(api);
        if (!vectorDataDrawingApi) {
            return false;
        }
        vectorDataDrawingApi_ = vectorDataDrawingApi;
    } else if (id.capabilityName() == VEHICLE_COLLECTION_API_CAPABILITY_NAME) {
        auto vehicleCollectionApi = dynamic_cast<LmCdl::I_VehicleCollectionApi*>(api);
        if (!vehicleCollectionApi) {
            return false;
        }
        vehicleCollectionApi_ = vehicleCollectionApi;
    }

    if (allRequirementsProvided()) {
        createPluginContent();
    }
    return true;
}

QObject* DynamicVectorDataDrawingPlugin::getProvidedApi() { return nullptr; }

bool DynamicVectorDataDrawingPlugin::isFullyInitialized() const { return content_; }

bool DynamicVectorDataDrawingPlugin::allRequirementsProvided() const
{
    return vectorDataDrawingApi_ && vehicleCollectionApi_;
}

void DynamicVectorDataDrawingPlugin::createPluginContent()
{
    content_.reset(new DynamicVectorDataDrawingPluginContent(*vectorDataDrawingApi_, *vehicleCollectionApi_));
}
