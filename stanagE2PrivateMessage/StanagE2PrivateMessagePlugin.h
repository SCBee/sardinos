/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class StanagE2PrivateMessageContentCreator;

namespace LmCdl {
class I_VehicleCollectionApi;
class I_VcsiApplicationApi;
}

class StanagE2PrivateMessagePlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    explicit StanagE2PrivateMessagePlugin();
    virtual ~StanagE2PrivateMessagePlugin();

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

    QScopedPointer<StanagE2PrivateMessageContentCreator> vehicleContentCreator_;
};
