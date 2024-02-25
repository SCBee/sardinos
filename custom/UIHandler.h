#pragma once

#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_MissionDrawingApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <MissionDomain.h>
#include <MissionPlanningDrawing.h>

class UIHandler
{
public:
    enum State
    {
        STARTUP,
        CanGetFlightPath,
        CannotGetFlightPath,
        CanRunMission,
        CanCancelMission,
    };

    void initContextMenuItems(LmCdl::I_ContextMenuItem& missionBoundMenuItem_,
                              LmCdl::I_ContextMenuItem& submitMissionMenuItem_);
    [[maybe_unused]] void changeUI(
        State& newState,
        LmCdl::I_ContextMenuItem& missionBoundMenuItem_,
        LmCdl::I_ContextMenuItem& submitMissionMenuItem_,
        MissionDomain& mission_,
        LmCdl::I_MissionDrawingApi& missionApi_,
        LmCdl::I_VectorDataDrawingApi& drawApi_);
    void updateUIState(const State& newState,
                       State& m_state,
                       LmCdl::I_ContextMenuItem& missionBoundMenuItem_,
                       LmCdl::I_ContextMenuItem& submitMissionMenuItem_,
                       MissionDomain& mission_,
                       LmCdl::I_MissionDrawingApi& missionApi_,
                       LmCdl::I_VectorDataDrawingApi& drawApi_);

private:
    MissionPlanningDrawing* drawing_ = new MissionPlanningDrawing();
};
