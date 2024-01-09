/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QList>
#include <QObject>
#include <QScopedPointer>

namespace LmCdl {
class I_PointOfInterestApi;
class PluginCapabilityIdentifier;
class PluginRequirement;
}
class AnimalTrackerContent;

class AnimalTrackerPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    explicit AnimalTrackerPlugin();
    virtual ~AnimalTrackerPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const;
    LmCdl::PluginCapabilityIdentifier providedApi() const;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api);
    QObject* getProvidedApi();
    bool isFullyInitialized() const;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(AnimalTrackerPlugin);

    LmCdl::I_PointOfInterestApi* pointOfInterestApi_;

    QScopedPointer<AnimalTrackerContent> pluginContent_;
};
