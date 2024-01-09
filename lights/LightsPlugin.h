/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LightsContainerRegistry.h>
#include <LmCdl/I_Plugin.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QScopedPointer>

namespace LmCdl {
class I_ActionApi;
class I_QmlApi;
class I_VehicleCollectionApi;
class I_VcsiApplicationApi;
}

/* This class defines the interface between VCSi and the plugin.  VCSi will
 * dynamically load this plugin at application startup and will inject
 * API classes into this class based on the plugin interfaces that it
 * declares as required.
 *
 * The LmCdl::I_VehicleCollectionApi interface allows this plugin to:
 *
 * \li Discover when vehicles are discovered/removed.
 * \li Access the vehicle's type/subtype.
 * \li Access whether this instance of VCSi is in control of this vehicle.
 * \li Send/receive STANAG 4586 Edition 2 Amendment 1 public/private messages to
 * the vehicle.
 *
 * The LmCdl::I_VcsiApplicationApi interface allows this plugin to:
 *
 * \li Add User Interface controls to the Vehicle panel.
 * \li Create user notifications when the engine is acting outside of its
 * nominal operating conditions.
 *
 * The LmCdl::I_QmlApi interface allows this plugin to:
 *
 * \li Make use of the common QQmlEngine object, so that Ui operations run
 * efficiently, even as more plugins are added to the system.
 *
 * VCSi will inject these interfaces in a non-deterministic order, hence it may
 * be important to wait until every interface is injected before starting to
 * create/activate the plugin's functionality.  This example below accomplishes
 * this by invoking startPluginIfInitialized() after each API class is injected.
 */
class LightsPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    explicit LightsPlugin();
    ~LightsPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private:
    bool hasAllDependencies() const;
    void startPluginIfInitialized();

private:
    LmCdl::I_VehicleCollectionApi* vehicleCollectionApi_;
    LmCdl::I_VcsiApplicationApi* applicationApi_;
    LmCdl::I_QmlApi* qmlApi_;
    LmCdl::I_ActionApi* actionApi_;

    QScopedPointer<LightsContainerRegistry> containerRegistry_;
};
