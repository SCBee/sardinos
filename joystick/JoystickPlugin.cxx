/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#include <JoystickPlugin.h>

#include <JoystickDisplay.h>
#include <LmCdl/I_HardwareCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

JoystickPlugin::JoystickPlugin()
    : hardwareCollectionApi_(nullptr)
{
}

JoystickPlugin::~JoystickPlugin() { }

QList<LmCdl::PluginRequirement> JoystickPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(HARDWARE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier JoystickPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool JoystickPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == HARDWARE_COLLECTION_API_CAPABILITY_NAME) {
        hardwareCollectionApi_ = dynamic_cast<LmCdl::I_HardwareCollectionApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* JoystickPlugin::getProvidedApi() { return nullptr; }

bool JoystickPlugin::isFullyInitialized() const { return (hardwareCollectionApi_); }

void JoystickPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        joystickDisplay_.reset(new JoystickDisplay(*hardwareCollectionApi_));
    }
}
