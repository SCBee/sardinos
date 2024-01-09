/**
 \file
 Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class ZeroizeGcsDataExampleContent;

namespace LmCdl {
class I_VcsiStandardPaths;
class I_ZeroizeApi;
}

class ZeroizeGcsDataExamplePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    ZeroizeGcsDataExamplePlugin();
    ~ZeroizeGcsDataExamplePlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    bool allRequirementsProvided() const;
    void createPluginContent();

private:
    Q_DISABLE_COPY(ZeroizeGcsDataExamplePlugin)

    LmCdl::I_ZeroizeApi* zeroizeApi_ = nullptr;
    LmCdl::I_VcsiStandardPaths* standardPathsApi_ = nullptr;

    QScopedPointer<ZeroizeGcsDataExampleContent> content_;
};
