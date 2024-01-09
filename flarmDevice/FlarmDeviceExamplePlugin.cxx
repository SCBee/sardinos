/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <FlarmDeviceExamplePlugin.h>

#include <FlarmDeviceExamplePluginContent.h>
#include <LmCdl/I_FlarmDevice.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

FlarmDeviceExamplePlugin::FlarmDeviceExamplePlugin() { }

FlarmDeviceExamplePlugin::~FlarmDeviceExamplePlugin() { }

QList<LmCdl::PluginRequirement> FlarmDeviceExamplePlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(FLARM_DEVICE_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier FlarmDeviceExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool FlarmDeviceExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == FLARM_DEVICE_CAPABILITY_NAME) {
        flarmDevice_ = dynamic_cast<LmCdl::I_FlarmDevice*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* FlarmDeviceExamplePlugin::getProvidedApi() { return nullptr; }

bool FlarmDeviceExamplePlugin::isFullyInitialized() const { return (flarmDevice_); }

void FlarmDeviceExamplePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new FlarmDeviceExamplePluginContent(*flarmDevice_));
    }
}
