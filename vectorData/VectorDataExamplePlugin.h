/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_AggregateVectorDataLoader.h>
#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class VectorDataExampleContent;
namespace LmCdl {
class I_AggregateVectorDataLoader;
class I_CenterMapOnLocationApi;
class I_VcsiApplicationApi;
class I_VcsiStandardPaths;
class I_VectorDataLayerRegistry;
class I_VectorDataLoaderRegistry;
class I_VectorDataWriterRegistry;
}

class VectorDataExamplePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    VectorDataExamplePlugin();
    virtual ~VectorDataExamplePlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const;
    LmCdl::PluginCapabilityIdentifier providedApi() const;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api);
    QObject* getProvidedApi();
    bool isFullyInitialized() const;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(VectorDataExamplePlugin);

    LmCdl::I_VectorDataLoaderRegistry* loaderRegistry_;
    LmCdl::I_VectorDataLayerRegistry* layerRegistry_;
    LmCdl::I_VectorDataWriterRegistry* vectorDataWriters_;
    LmCdl::I_AggregateVectorDataLoader* mainDataLoader_;
    LmCdl::I_CenterMapOnLocationApi* centerMapApi_;
    LmCdl::I_VcsiApplicationApi* applicationApi_;
    LmCdl::I_VcsiStandardPaths* standardPathsApi_;
};
