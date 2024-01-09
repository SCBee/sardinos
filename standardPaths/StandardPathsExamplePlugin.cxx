/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <StandardPathsExamplePlugin.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiStandardPaths.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>
#include <StandardPathsExampleContent.h>
#include <StandardPathsExampleView.h>

StandardPathsExamplePlugin::StandardPathsExamplePlugin()
    : standardPaths_(nullptr)
    , applicationApi_(nullptr)
{
}

StandardPathsExamplePlugin::~StandardPathsExamplePlugin() { }

QList<LmCdl::PluginRequirement> StandardPathsExamplePlugin::requiredApis() const
{
    return {
        LmCdl::PluginRequirement(VCSI_STANDARD_PATHS_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
    };
}

LmCdl::PluginCapabilityIdentifier StandardPathsExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier();
}

bool StandardPathsExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VCSI_STANDARD_PATHS_CAPABILITY_NAME) {
        standardPaths_ = dynamic_cast<LmCdl::I_VcsiStandardPaths*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }
    startPluginIfInitialized();
    return capabilityFound;
}

QObject* StandardPathsExamplePlugin::getProvidedApi()
{
    if (isFullyInitialized()) {
        return pluginContent_.data();
    }
    return nullptr;
}

bool StandardPathsExamplePlugin::isFullyInitialized() const { return (standardPaths_ && applicationApi_); }

void StandardPathsExamplePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new StandardPathsExampleContent(*standardPaths_));
        view_.reset(new StandardPathsExampleView(*applicationApi_, *standardPaths_, *pluginContent_));
    }
}
