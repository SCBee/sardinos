/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QPointer>
#include <QScopedPointer>

class FlarmDeviceExamplePluginContent;

namespace LmCdl {
class I_FlarmDevice;
}

class FlarmDeviceExamplePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    FlarmDeviceExamplePlugin();
    virtual ~FlarmDeviceExamplePlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(FlarmDeviceExamplePlugin);

    QPointer<LmCdl::I_FlarmDevice> flarmDevice_;

    QScopedPointer<FlarmDeviceExamplePluginContent> pluginContent_;
};
