/**
\file
Copyright (c) 2016 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>

class BatteryContent;
class VehicleStatusContent;

namespace LmCdl {
class I_QmlApi;
class I_UnitPreferencesApi;
class I_VcsiApplicationApi;
class I_VehicleCollectionApi;
}

class VehicleTabPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    explicit VehicleTabPlugin();
    virtual ~VehicleTabPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;

    QObject* getProvidedApi() override;

    bool isFullyInitialized() const override;

private:
    Q_DISABLE_COPY(VehicleTabPlugin)

    bool allDependenciesProvided() const;
    void createPluginContent();

    LmCdl::I_VehicleCollectionApi* vehicleApi_;
    LmCdl::I_VcsiApplicationApi* applicationApi_;
    LmCdl::I_UnitPreferencesApi* unitPreferencesApi_;
    LmCdl::I_QmlApi* qmlApi_;

    QScopedPointer<VehicleStatusContent> statusContent_;
    QScopedPointer<BatteryContent> batteryContent_;
};
