#pragma once

#include <QtGlobal>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_ContextMenu.h>

class MissionPlanningContent {
public:
    explicit MissionPlanningContent(LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_PointOfInterestApi& poiApi);
    virtual ~MissionPlanningContent();

private:
    Q_DISABLE_COPY(MissionPlanningContent)
    void Initialize();
    void publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId sourceId, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest);
    void connectToApiSignals();


private:
    LmCdl::I_ContextMenuItem& contextMenuItem_;
    LmCdl::I_PointOfInterestApi& poiApi_;
    QHash<LmCdl::VcsiPointOfInterestId, LmCdl::VcsiPointOfInterestId> sourceIdsToClonedIds_;

};
