#include "UIHandler.h"

#include <qcolor.h>

void UIHandler::initContextMenuItems(
    LmCdl::I_ContextMenuItem& missionBoundMenuItem,
    LmCdl::I_ContextMenuItem& submitMissionMenuItem,
    LmCdl::I_ContextMenuItem& forceLandMenuItem)
{
    missionBoundMenuItem.setBackgroundColor(Qt::cyan);
    missionBoundMenuItem.setDescription("Add Mission Bound");
    missionBoundMenuItem.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    missionBoundMenuItem.setIcon(":/MissionPlanning/SelectIcon");
    missionBoundMenuItem.setVisible(true);

    submitMissionMenuItem.setBackgroundColor(Qt::blue);
    submitMissionMenuItem.setDescription("Get Flight Path");
    submitMissionMenuItem.setGrouping(LmCdl::ContextMenuItemGrouping::Top);
    submitMissionMenuItem.setIcon(":/MissionPlanning/MissionIcon");
    submitMissionMenuItem.setVisible(false);

    forceLandMenuItem.setBackgroundColor(Qt::red);
    forceLandMenuItem.setDescription("Force Land");
    forceLandMenuItem.setGrouping(LmCdl::ContextMenuItemGrouping::Top);
    forceLandMenuItem.setIcon(":/MissionPlanning/LandIcon");
    forceLandMenuItem.setVisible(false);
}

void UIHandler::changeUI(State& newState,
                         LmCdl::I_ContextMenuItem& missionBoundMenuItem,
                         LmCdl::I_ContextMenuItem& submitMissionMenuItem,
                         MissionDomain& mission,
                         LmCdl::I_MissionDrawingApi& missionApi,
                         LmCdl::I_VectorDataDrawingApi& drawApi)
{
    switch (newState) {
        case State::CanGetFlightPath:
            missionBoundMenuItem.setVisible(true);
            submitMissionMenuItem.setVisible(true);
            submitMissionMenuItem.setBackgroundColor(Qt::blue);
            submitMissionMenuItem.setDescription("Get Flight Path");
            submitMissionMenuItem.setIcon(":/MissionPlanning/PathIcon");
            drawing_->clearFlightPath(mission, missionApi);
            break;
        case State::CannotGetFlightPath:
            missionBoundMenuItem.setVisible(true);
            submitMissionMenuItem.setVisible(false);
            drawing_->clearFlightPath(mission, missionApi);
            break;
        case State::CanRunMission:
            missionBoundMenuItem.setVisible(true);
            submitMissionMenuItem.setVisible(true);
            submitMissionMenuItem.setBackgroundColor(Qt::green);
            submitMissionMenuItem.setDescription("Begin Mission");
            submitMissionMenuItem.setIcon(":/MissionPlanning/MissionIcon");
            drawing_->clearMissionArea(drawApi);
            break;
        case State::CanCancelMission:
            missionBoundMenuItem.setVisible(false);
            submitMissionMenuItem.setVisible(true);
            submitMissionMenuItem.setBackgroundColor(Qt::red);
            submitMissionMenuItem.setDescription("Cancel Mission");
            submitMissionMenuItem.setIcon(":/MissionPlanning/CancelIcon");
            drawing_->clearMissionArea(drawApi);
            break;
        default:
            missionBoundMenuItem.setVisible(true);
            submitMissionMenuItem.setVisible(true);
            submitMissionMenuItem.setBackgroundColor(Qt::blue);
            submitMissionMenuItem.setDescription("Get Flight Path");
            submitMissionMenuItem.setIcon(":/MissionPlanning/PathIcon");
            drawing_->clearFlightPath(mission, missionApi);
            break;
    }
}

void UIHandler::updateUIState(const State& newState,
                              State& m_state,
                              LmCdl::I_ContextMenuItem& missionBoundMenuItem,
                              LmCdl::I_ContextMenuItem& submitMissionMenuItem,
                              MissionDomain& mission,
                              LmCdl::I_MissionDrawingApi& missionApi,
                              LmCdl::I_VectorDataDrawingApi& drawApi)
{
    if (newState != m_state) {
        m_state = newState;
        changeUI(m_state,
                 missionBoundMenuItem,
                 submitMissionMenuItem,
                 mission,
                 missionApi,
                 drawApi);
    }
}