/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_KeepOutRestrictionZoneCollection.h>
#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>
class RestrictionZoneApiExampleContent;
class RestrictionZoneApiExampleView;
namespace LmCdl {
class I_AggregateVectorDataLoader;
class I_KeepOutRestrictionZone;
class I_KeepOutRestrictionZoneCollection;
class I_VcsiApplicationApi;
class I_VectorDataLayerRegistry;
}
class RestrictionZoneApiExamplePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    RestrictionZoneApiExamplePlugin();
    virtual ~RestrictionZoneApiExamplePlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(RestrictionZoneApiExamplePlugin);

    LmCdl::I_KeepOutRestrictionZoneCollection* keepOutRestrictionZoneCollectionApi_;
    LmCdl::I_VectorDataLayerRegistry* layerRegistry_;
    LmCdl::I_AggregateVectorDataLoader* mainDataLoader_;
    LmCdl::I_VcsiApplicationApi* applicationApi_;

    QScopedPointer<RestrictionZoneApiExampleContent> restrictionZoneApiExampleContent_;
    QScopedPointer<RestrictionZoneApiExampleView> restrictionZoneApiExampleView_;
};
