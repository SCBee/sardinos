/**
\file
Copyright (c) 2016 Lockheed Martin Corp. All rights reserved.
*/

#include <VehicleTabPlugin.h>

#include <BatteryContent.h>
#include <LmCdl/I_QmlApi.h>
#include <LmCdl/I_UnitPreferencesApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <VehicleStatusContent.h>

VehicleTabPlugin::VehicleTabPlugin()
    : vehicleApi_(nullptr)
    , applicationApi_(nullptr)
    , unitPreferencesApi_(nullptr)
    , qmlApi_(nullptr)
{
}

VehicleTabPlugin::~VehicleTabPlugin() = default;

QList<LmCdl::PluginRequirement> VehicleTabPlugin::requiredApis() const
{
    return {
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VEHICLE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(UNIT_PREFERENCES_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(QML_API_CAPABILITY_NAME, 1, 0, 0),
    };
}

LmCdl::PluginCapabilityIdentifier VehicleTabPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool VehicleTabPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        auto* applicationApi = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        if (!applicationApi) {
            return false;
        }
        applicationApi_ = applicationApi;
    } else if (id.capabilityName() == VEHICLE_COLLECTION_API_CAPABILITY_NAME) {
        auto* vehicleApi = dynamic_cast<LmCdl::I_VehicleCollectionApi*>(api);
        if (!vehicleApi) {
            return false;
        }
        vehicleApi_ = vehicleApi;
    } else if (id.capabilityName() == UNIT_PREFERENCES_API_CAPABILITY_NAME) {
        auto* unitPreferencesApi = dynamic_cast<LmCdl::I_UnitPreferencesApi*>(api);
        if (!unitPreferencesApi) {
            return false;
        }
        unitPreferencesApi_ = unitPreferencesApi;
    } else if (id.capabilityName() == QML_API_CAPABILITY_NAME) {
        auto qmlApi = dynamic_cast<LmCdl::I_QmlApi*>(api);
        if (!qmlApi) {
            return false;
        }
        qmlApi_ = qmlApi;
    } else {
        return false;
    }

    if (allDependenciesProvided()) {
        createPluginContent();
    }
    return true;
}

QObject* VehicleTabPlugin::getProvidedApi() { return nullptr; }

bool VehicleTabPlugin::isFullyInitialized() const { return statusContent_; }

bool VehicleTabPlugin::allDependenciesProvided() const
{
    return vehicleApi_ && applicationApi_ && unitPreferencesApi_ && qmlApi_;
}

void VehicleTabPlugin::createPluginContent()
{
    statusContent_.reset(new VehicleStatusContent(
        *vehicleApi_, applicationApi_->widgetExtensionApi().vehicleApi(), *unitPreferencesApi_));

    batteryContent_.reset(
        new BatteryContent(*vehicleApi_, applicationApi_->widgetExtensionApi().vehicleApi(), *qmlApi_));
}
