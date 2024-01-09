/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QScopedPointer>
#include <QtGlobal>

class VideoStreamContent;

namespace LmCdl {
class I_VideoStreamApiCollection;
class I_InteractableVideoDisplayApi;
}

class VideoStreamPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    VideoStreamPlugin();
    virtual ~VideoStreamPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(VideoStreamPlugin);

    LmCdl::I_VideoStreamApiCollection* videoStreamApiCollection_;
    LmCdl::I_InteractableVideoDisplayApi* interactableVideoDisplayApi_;

    QScopedPointer<VideoStreamContent> pluginContent_;
};
