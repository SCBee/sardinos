#pragma once

#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_MissionDrawingApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <MissionDomain.h>
#include <MissionPlanningDrawing.h>

class UIHandler {
public:
    enum State
    {
        STARTUP,
        CanGetFlightPath,
        CannotGetFlightPath,
        CanRunMission,
        CanCancelMission,
    };

    void initContextMenuItems(LmCdl::I_ContextMenuItem &missionBoundMenuItem, LmCdl::I_ContextMenuItem &submitMissionMenuItem, LmCdl::I_ContextMenuItem &forceLandMenuItem);
    void changeUI(State& newState, LmCdl::I_ContextMenuItem& missionBoundMenuItem, LmCdl::I_ContextMenuItem& submitMissionMenuItem, MissionDomain& mission, LmCdl::I_MissionDrawingApi& missionApi, LmCdl::I_VectorDataDrawingApi& drawApi);
    void updateUIState(const State& newState, State& m_state, LmCdl::I_ContextMenuItem& missionBoundMenuItem, LmCdl::I_ContextMenuItem& submitMissionMenuItem, MissionDomain& mission, LmCdl::I_MissionDrawingApi& missionApi, LmCdl::I_VectorDataDrawingApi& drawApi);
private:
    MissionPlanningDrawing* drawing_ = new MissionPlanningDrawing();
};
