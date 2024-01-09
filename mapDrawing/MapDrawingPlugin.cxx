/**
\file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <MapDrawingPlugin.h>

#include <FaaObstacleContent.h>
#include <LmCdl/I_PanterraPublicApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

MapDrawingPlugin::MapDrawingPlugin()
    : panterraApi_(nullptr)
    , guiApi_(nullptr)
{
}

MapDrawingPlugin::~MapDrawingPlugin() { }

QList<LmCdl::PluginRequirement> MapDrawingPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(PANTERRA_PUBLIC_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier MapDrawingPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool MapDrawingPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == PANTERRA_PUBLIC_API_CAPABILITY_NAME) {
        panterraApi_ = dynamic_cast<LmCdl::I_PanterraPublicApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        guiApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* MapDrawingPlugin::getProvidedApi() { return nullptr; }

bool MapDrawingPlugin::isFullyInitialized() const { return (panterraApi_ && guiApi_); }

void MapDrawingPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new FaaObstacleContent(*panterraApi_, *guiApi_));
    }
}
