/**
 \file
 Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class PointOfInterestContent;

namespace LmCdl {
class I_PointOfInterestApi;
}

class PointOfInterestPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    PointOfInterestPlugin();
    virtual ~PointOfInterestPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(PointOfInterestPlugin);

    LmCdl::I_PointOfInterestApi* pointOfInterestApi_;

    QScopedPointer<PointOfInterestContent> pluginContent_;
};
