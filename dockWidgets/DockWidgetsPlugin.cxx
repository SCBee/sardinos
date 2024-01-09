/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <DockWidgetsPlugin.h>

#include <DockWidgetsContent.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

DockWidgetsPlugin::DockWidgetsPlugin()
    : vcsiApplicationApi_(nullptr)
{
}

DockWidgetsPlugin::~DockWidgetsPlugin() { }

QList<LmCdl::PluginRequirement> DockWidgetsPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier DockWidgetsPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool DockWidgetsPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        vcsiApplicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* DockWidgetsPlugin::getProvidedApi() { return nullptr; }

bool DockWidgetsPlugin::isFullyInitialized() const { return (vcsiApplicationApi_); }

void DockWidgetsPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        LmCdl::I_VcsiWidgetExtensionApi& widgetApi = vcsiApplicationApi_->widgetExtensionApi();
        pluginContent_.reset(
            new DockWidgetsContent(widgetApi.dockableApi(), widgetApi.vehicleApi(), widgetApi.menuBarApi()));
    }
}
