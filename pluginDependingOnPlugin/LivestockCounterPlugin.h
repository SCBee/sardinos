/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

namespace GdWild {
class I_AnimalTrackerApi;
}

class LivestockCounterContent;

class LivestockCounterPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    LivestockCounterPlugin();
    virtual ~LivestockCounterPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const;
    LmCdl::PluginCapabilityIdentifier providedApi() const;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api);
    QObject* getProvidedApi();
    bool isFullyInitialized() const;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(LivestockCounterPlugin);

    GdWild::I_AnimalTrackerApi* animalTrackerApi_;

    QScopedPointer<LivestockCounterContent> pluginContent_;
};
