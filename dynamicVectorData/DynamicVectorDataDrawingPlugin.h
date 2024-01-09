/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QPointer>
#include <QScopedPointer>
#include <QtGlobal>

class DynamicVectorDataDrawingPluginContent;
namespace LmCdl {
class I_VectorDataDrawingApi;
class I_VehicleCollectionApi;
class PluginCapabilityIdentifier;
class PluginRequirement;
}

class DynamicVectorDataDrawingPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    DynamicVectorDataDrawingPlugin();
    ~DynamicVectorDataDrawingPlugin() override;

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    bool allRequirementsProvided() const;
    void createPluginContent();

private:
    Q_DISABLE_COPY(DynamicVectorDataDrawingPlugin)

    QPointer<LmCdl::I_VectorDataDrawingApi> vectorDataDrawingApi_;
    QPointer<LmCdl::I_VehicleCollectionApi> vehicleCollectionApi_;

    QScopedPointer<DynamicVectorDataDrawingPluginContent> content_;
};
