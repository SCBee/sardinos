/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <RoutesPlugin.h>

#include <CustomWaypointEditingContent.h>
#include <CustomWaypointUploadContentCreator.h>
#include <ReadonlyRoutesContentCreator.h>

#include <LmCdl/I_RouteApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

RoutesPlugin::RoutesPlugin()
    : vehicleApi_(nullptr)
    , applicationApi_(nullptr)
    , routeApi_(nullptr)
{
}

RoutesPlugin::~RoutesPlugin() { }

QList<LmCdl::PluginRequirement> RoutesPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VEHICLE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(ROUTE_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier RoutesPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool RoutesPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
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
    if (id.capabilityName() == ROUTE_API_CAPABILITY_NAME) {
        routeApi_ = dynamic_cast<LmCdl::I_RouteApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* RoutesPlugin::getProvidedApi() { return nullptr; }

bool RoutesPlugin::isFullyInitialized() const { return (vehicleApi_ && applicationApi_ && routeApi_); }

void RoutesPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        readonlyRoutesCreator_.reset(new ReadonlyRoutesContentCreator(
            *vehicleApi_, *routeApi_, applicationApi_->widgetExtensionApi().vehicleApi()));
        waypointEditingContent_.reset(new CustomWaypointEditingContent(*vehicleApi_, *routeApi_));
        waypointUploadCreator_.reset(new CustomWaypointUploadContentCreator(*vehicleApi_, *routeApi_));
    }
}
