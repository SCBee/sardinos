/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <TerrainProfilePlugin.h>

#include <LmCdl/I_GroundElevationApi.h>
#include <LmCdl/I_MapPickApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>
#include <TerrainProfileContent.h>

TerrainProfilePlugin::TerrainProfilePlugin()
    : applicationApi_(nullptr)
    , groundElevationApi_(nullptr)
    , mapPickApi_(nullptr)
{
}

TerrainProfilePlugin::~TerrainProfilePlugin() { }

QList<LmCdl::PluginRequirement> TerrainProfilePlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(GROUND_ELEVATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(MAP_PICK_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier TerrainProfilePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool TerrainProfilePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == GROUND_ELEVATION_API_CAPABILITY_NAME) {
        groundElevationApi_ = dynamic_cast<LmCdl::I_GroundElevationApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == MAP_PICK_API_CAPABILITY_NAME) {
        mapPickApi_ = dynamic_cast<LmCdl::I_MapPickApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* TerrainProfilePlugin::getProvidedApi() { return nullptr; }

bool TerrainProfilePlugin::isFullyInitialized() const
{
    return (applicationApi_ && groundElevationApi_ && mapPickApi_);
}

void TerrainProfilePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new TerrainProfileContent(applicationApi_->widgetExtensionApi().dockableApi(),
            applicationApi_->widgetExtensionApi().menuBarApi(), *groundElevationApi_, *mapPickApi_));
    }
}
