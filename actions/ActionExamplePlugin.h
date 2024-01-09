/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class ActionExamplePluginContent;
namespace LmCdl {
class I_ActionApi;
class I_QmlApi;
class I_VcsiApplicationApi;
}

class ActionExamplePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    ActionExamplePlugin();
    virtual ~ActionExamplePlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(ActionExamplePlugin)

    LmCdl::I_ActionApi* actionApi_;
    LmCdl::I_VcsiApplicationApi* applicationApi_;
    LmCdl::I_QmlApi* qmlApi_;

    QScopedPointer<ActionExamplePluginContent> pluginContent_;
};
