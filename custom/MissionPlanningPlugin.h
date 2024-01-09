#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QScopedPointer>

class MissionPlanningContentCreator;

namespace LmCdl {
class I_VectorDataDrawingApi;
class I_PointOfInterestApi;
class I_VcsiApplicationApi;
}

class MissionPlanningPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    MissionPlanningPlugin();

    virtual ~MissionPlanningPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;

    LmCdl::PluginCapabilityIdentifier providedApi() const override;

    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject *api) override;

    QObject *getProvidedApi() override;

    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

private:
    Q_DISABLE_COPY(MissionPlanningPlugin);

    LmCdl::I_PointOfInterestApi* pointOfInterestApi_;

    LmCdl::I_VcsiApplicationApi* applicationApi_;

    LmCdl::I_VectorDataDrawingApi* vectorDrawingApi_;

    QScopedPointer<MissionPlanningContentCreator> pluginContentCreator_;
};
