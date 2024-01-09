/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class TerrainProfileContent;

namespace LmCdl {
class I_VcsiApplicationApi;
class I_GroundElevationApi;
class I_MapPickApi;
}

class TerrainProfilePlugin : public QObject,
                             public LmCdl::I_Plugin

{
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    TerrainProfilePlugin();
    virtual ~TerrainProfilePlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(TerrainProfilePlugin);

    LmCdl::I_VcsiApplicationApi* applicationApi_;
    LmCdl::I_GroundElevationApi* groundElevationApi_;
    LmCdl::I_MapPickApi* mapPickApi_;

    QScopedPointer<TerrainProfileContent> pluginContent_;
};
