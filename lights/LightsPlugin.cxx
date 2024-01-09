/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsPlugin.h>

#include <LmCdl/I_ActionApi.h>
#include <LmCdl/I_QmlApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>

LightsPlugin::LightsPlugin()
    : vehicleCollectionApi_(nullptr)
    , applicationApi_(nullptr)
    , qmlApi_(nullptr)
    , actionApi_(nullptr)
{
}

LightsPlugin::~LightsPlugin() { }

QList<LmCdl::PluginRequirement> LightsPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VEHICLE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(QML_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(ACTION_CAPABILITY_NAME, ACTION_VERSION_1) };
}

LmCdl::PluginCapabilityIdentifier LightsPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool LightsPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VEHICLE_COLLECTION_API_CAPABILITY_NAME) {
        vehicleCollectionApi_ = dynamic_cast<LmCdl::I_VehicleCollectionApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == QML_API_CAPABILITY_NAME) {
        qmlApi_ = dynamic_cast<LmCdl::I_QmlApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == ACTION_CAPABILITY_NAME) {
        actionApi_ = dynamic_cast<LmCdl::I_ActionApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* LightsPlugin::getProvidedApi() { return nullptr; }

bool LightsPlugin::isFullyInitialized() const { return (!containerRegistry_.isNull()); }

bool LightsPlugin::hasAllDependencies() const
{
    return (vehicleCollectionApi_ && applicationApi_ && qmlApi_ && actionApi_);
}

/* Once all the APIs are injected, this function initializes the
 * plugin's functionality.
 */
void LightsPlugin::startPluginIfInitialized()
{
    if (hasAllDependencies() && !isFullyInitialized()) {
        containerRegistry_.reset(
            new LightsContainerRegistry(*vehicleCollectionApi_, *applicationApi_, *qmlApi_, *actionApi_));
    }
}
