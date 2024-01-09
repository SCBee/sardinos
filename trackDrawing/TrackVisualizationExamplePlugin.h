/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class TrackVisualizationExamplePluginContent;

namespace LmCdl {
class PluginCapabilityIdentifier;
class PluginRequirement;
class I_TrackDrawingApi;
}

class TrackVisualizationExamplePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    TrackVisualizationExamplePlugin();
    ~TrackVisualizationExamplePlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    bool allRequirementsProvided() const;
    void createPluginContent();

    LmCdl::I_TrackDrawingApi* trackDrawingApi_;
    QScopedPointer<TrackVisualizationExamplePluginContent> content_;
};