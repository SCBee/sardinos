/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class StandardPathsExampleContent;
class StandardPathsExampleView;

namespace LmCdl {
class I_VcsiApplicationApi;
class I_VcsiStandardPaths;
}

class StandardPathsExamplePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    StandardPathsExamplePlugin();
    virtual ~StandardPathsExamplePlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const;
    LmCdl::PluginCapabilityIdentifier providedApi() const;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api);
    QObject* getProvidedApi();
    bool isFullyInitialized() const;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(StandardPathsExamplePlugin);

    LmCdl::I_VcsiStandardPaths* standardPaths_;
    LmCdl::I_VcsiApplicationApi* applicationApi_;

    QScopedPointer<StandardPathsExampleContent> pluginContent_;
    QScopedPointer<StandardPathsExampleView> view_;
};
