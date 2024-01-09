/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QPointer>
#include <QtGlobal>

class PreflightChecklistExampleContent;

namespace LmCdl {
class I_PreflightChecklistApi;
class I_PreflightChecklistVisualizationApi;
class PluginCapabilityIdentifier;
class PluginRequirement;
}

class PreflightChecklistExamplePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

public:
    explicit PreflightChecklistExamplePlugin();
    ~PreflightChecklistExamplePlugin() override;

private:
    bool allRequirementsProvided() const;
    void createPluginContent();

private:
    Q_DISABLE_COPY(PreflightChecklistExamplePlugin)

    QPointer<LmCdl::I_PreflightChecklistApi> preflightChecklistApi_;
    QPointer<LmCdl::I_PreflightChecklistVisualizationApi> preflightChecklistVisualizationApi_;

    QScopedPointer<PreflightChecklistExampleContent> content_;
};
