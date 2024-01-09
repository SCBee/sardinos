/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QPointer>
#include <QScopedPointer>

class ExamplePayloadDrawingApiPluginContent;

namespace LmCdl {
class I_GroundElevationApi;
class I_PayloadDrawingApi;
class PluginCapabilityIdentifier;
class PluginRequirement;
}

class ExamplePayloadDrawingApiPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    ExamplePayloadDrawingApiPlugin();
    ~ExamplePayloadDrawingApiPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    bool allRequirementsProvided() const;
    void createPluginContent();

    QPointer<LmCdl::I_PayloadDrawingApi> payloadDrawingApi_;
    QPointer<LmCdl::I_GroundElevationApi> groundElevationApi_;

    QScopedPointer<ExamplePayloadDrawingApiPluginContent> content_;
};
