/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class NotificationContentCreator;

namespace LmCdl {
class I_VehicleCollectionApi;
class I_VcsiApplicationApi;
}

class NotificationPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    NotificationPlugin();
    virtual ~NotificationPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(NotificationPlugin)

    LmCdl::I_VehicleCollectionApi* vehicleApi_;
    LmCdl::I_VcsiApplicationApi* applicationApi_;

    QScopedPointer<NotificationContentCreator> contentCreator_;
};
