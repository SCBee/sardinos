/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#include <PreflightChecklistExamplePlugin.h>

#include <LmCdl/I_PreflightChecklistApi.h>
#include <LmCdl/I_PreflightChecklistVisualizationApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <PreflightChecklistExampleContent.h>

PreflightChecklistExamplePlugin::PreflightChecklistExamplePlugin() { }

PreflightChecklistExamplePlugin::~PreflightChecklistExamplePlugin() { }

QList<LmCdl::PluginRequirement> PreflightChecklistExamplePlugin::requiredApis() const
{
    return {
        LmCdl::PluginRequirement(PREFLIGHT_CHECKLIST_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(PREFLIGHT_CHECKLIST_VISUALIZATION_API_CAPABILITY_NAME, 1, 0, 0),
    };
}

LmCdl::PluginCapabilityIdentifier PreflightChecklistExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier();
}

bool PreflightChecklistExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    if (id.capabilityName() == PREFLIGHT_CHECKLIST_API_CAPABILITY_NAME) {
        auto preflightChecklistApi = dynamic_cast<LmCdl::I_PreflightChecklistApi*>(api);
        if (!preflightChecklistApi) {
            return false;
        }
        preflightChecklistApi_ = preflightChecklistApi;
    } else if (id.capabilityName() == PREFLIGHT_CHECKLIST_VISUALIZATION_API_CAPABILITY_NAME) {
        auto preflightChecklistVisualizationApi = dynamic_cast<LmCdl::I_PreflightChecklistVisualizationApi*>(api);
        if (!preflightChecklistVisualizationApi) {
            return false;
        }
        preflightChecklistVisualizationApi_ = preflightChecklistVisualizationApi;
    }

    if (allRequirementsProvided()) {
        createPluginContent();
    }
    return true;
}

QObject* PreflightChecklistExamplePlugin::getProvidedApi() { return nullptr; }

bool PreflightChecklistExamplePlugin::isFullyInitialized() const { return content_; }

bool PreflightChecklistExamplePlugin::allRequirementsProvided() const
{
    return preflightChecklistApi_ && preflightChecklistVisualizationApi_;
}

void PreflightChecklistExamplePlugin::createPluginContent()
{
    content_.reset(new PreflightChecklistExampleContent(*preflightChecklistApi_, *preflightChecklistVisualizationApi_));
}
