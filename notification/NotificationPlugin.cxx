/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <NotificationPlugin.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <NotificationContentCreator.h>
#include <QList>

NotificationPlugin::NotificationPlugin()
    : vehicleApi_(nullptr)
    , applicationApi_(nullptr)
{
}

NotificationPlugin::~NotificationPlugin() { }

QList<LmCdl::PluginRequirement> NotificationPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VEHICLE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier NotificationPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool NotificationPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
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

QObject* NotificationPlugin::getProvidedApi() { return nullptr; }

bool NotificationPlugin::isFullyInitialized() const { return (vehicleApi_ && applicationApi_); }

void NotificationPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        contentCreator_.reset(new NotificationContentCreator(*vehicleApi_, applicationApi_->userNotificationApi()));
    }
}
