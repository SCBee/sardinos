/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class FaaObstacleContent;

namespace LmCdl {
class I_PanterraPublicApi;
class I_VcsiApplicationApi;
}

class MapDrawingPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    explicit MapDrawingPlugin();
    virtual ~MapDrawingPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    LmCdl::I_PanterraPublicApi* panterraApi_;
    LmCdl::I_VcsiApplicationApi* guiApi_;

    QScopedPointer<FaaObstacleContent> pluginContent_;
};
