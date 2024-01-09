/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QScopedPointer>

class ReadonlyRoutesContentCreator;
class CustomWaypointEditingContent;
class CustomWaypointUploadContentCreator;

namespace LmCdl {
class I_VehicleCollectionApi;
class I_RouteApi;
class I_VcsiApplicationApi;
}

class RoutesPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    explicit RoutesPlugin();
    ~RoutesPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    LmCdl::I_VehicleCollectionApi* vehicleApi_;
    LmCdl::I_VcsiApplicationApi* applicationApi_;
    LmCdl::I_RouteApi* routeApi_;

    QScopedPointer<ReadonlyRoutesContentCreator> readonlyRoutesCreator_;
    QScopedPointer<CustomWaypointEditingContent> waypointEditingContent_;
    QScopedPointer<CustomWaypointUploadContentCreator> waypointUploadCreator_;
};
