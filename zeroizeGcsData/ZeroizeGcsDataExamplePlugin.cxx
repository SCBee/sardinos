/**
 \file
 Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
 */

#include <ZeroizeGcsDataExamplePlugin.h>

#include <LmCdl/I_VcsiStandardPaths.h>
#include <LmCdl/I_ZeroizeApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <ZeroizeGcsDataExampleContent.h>

ZeroizeGcsDataExamplePlugin::ZeroizeGcsDataExamplePlugin() { }

ZeroizeGcsDataExamplePlugin::~ZeroizeGcsDataExamplePlugin() = default;

QList<LmCdl::PluginRequirement> ZeroizeGcsDataExamplePlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(ZEROIZE_CAPABILITY_NAME, ZEROIZE_VERSION_1),
        LmCdl::PluginRequirement(VCSI_STANDARD_PATHS_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier ZeroizeGcsDataExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier();
}

bool ZeroizeGcsDataExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    if (id.capabilityName() == ZEROIZE_CAPABILITY_NAME) {
        auto zeroizeApi = dynamic_cast<LmCdl::I_ZeroizeApi*>(api);
        if (!zeroizeApi) {
            return false;
        }
        zeroizeApi_ = zeroizeApi;
    } else if (id.capabilityName() == VCSI_STANDARD_PATHS_CAPABILITY_NAME) {
        auto standardPathsApi = dynamic_cast<LmCdl::I_VcsiStandardPaths*>(api);
        if (!standardPathsApi) {
            return false;
        }
        standardPathsApi_ = standardPathsApi;
    } else {
        return false;
    }

    if (allRequirementsProvided()) {
        createPluginContent();
    }
    return true;
}

QObject* ZeroizeGcsDataExamplePlugin::getProvidedApi() { return nullptr; }

bool ZeroizeGcsDataExamplePlugin::isFullyInitialized() const { return content_; }

bool ZeroizeGcsDataExamplePlugin::allRequirementsProvided() const { return zeroizeApi_ && standardPathsApi_; }

void ZeroizeGcsDataExamplePlugin::createPluginContent()
{
    content_.reset(new ZeroizeGcsDataExampleContent(zeroizeApi_->zeroizableComponentApi(), *standardPathsApi_));
}
