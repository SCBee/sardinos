#pragma once

#include <QObject>
#include <QScopedPointer>

#include <LmCdl/I_Plugin.h>

#define DEBUG
#ifdef DEBUG
#    include <cstdio>

#    include <Windows.h>
#endif

class ContentCreator;

namespace LmCdl
{
    class I_VectorDataDrawingApi;
    class I_PointOfInterestApi;
    class I_VcsiApplicationApi;
    class I_MissionDrawingApi;
    class I_RouteApi;
    class I_TrackDrawingApi;
    class I_VideoStreamApiCollection;
}  // namespace LmCdl

class MissionPlanningPlugin
    : public QObject
    , public LmCdl::I_Plugin
{
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    MissionPlanningPlugin();

    ~MissionPlanningPlugin() override;

    [[nodiscard]] QList<LmCdl::PluginRequirement> requiredApis() const override;

    [[nodiscard]] LmCdl::PluginCapabilityIdentifier providedApi()
        const override;

    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id,
                        QObject* api) override;

    QObject* getProvidedApi() override;

    [[nodiscard]] bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(MissionPlanningPlugin);

    LmCdl::I_PointOfInterestApi* pointOfInterestApi_;

    LmCdl::I_VcsiApplicationApi* applicationApi_;

    LmCdl::I_VectorDataDrawingApi* vectorDrawingApi_;

    LmCdl::I_MissionDrawingApi* missionDrawingApi_;

    LmCdl::I_RouteApi* routeApi_;

    LmCdl::I_TrackDrawingApi* trackApi_;

    LmCdl::I_VideoStreamApiCollection* videoCollectionApi_;

    QScopedPointer<ContentCreator> pluginContentCreator_;
};
