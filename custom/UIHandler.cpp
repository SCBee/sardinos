#include <qcolor.h>

#include "UIHandler.h"

void UIHandler::initContextMenuItems(LmCdl::I_ContextMenuItem &missionBoundMenuItem_, LmCdl::I_ContextMenuItem &submitMissionMenuItem_)
{
    missionBoundMenuItem_.setBackgroundColor(Qt::cyan);
    missionBoundMenuItem_.setDescription("Add Mission Bound");
    missionBoundMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    missionBoundMenuItem_.setIcon(":/MissionPlanning/SelectIcon");
    missionBoundMenuItem_.setVisible(true);

    submitMissionMenuItem_.setBackgroundColor(Qt::blue);
    submitMissionMenuItem_.setDescription("Get Flight Path");
    submitMissionMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Top);
    submitMissionMenuItem_.setIcon(":/MissionPlanning/MissionIcon");
    submitMissionMenuItem_.setVisible(false);
}

[[maybe_unused]] void UIHandler::changeUI(State &newState, LmCdl::I_ContextMenuItem &missionBoundMenuItem_, LmCdl::I_ContextMenuItem &submitMissionMenuItem_, MissionDomain &mission_, LmCdl::I_MissionDrawingApi &missionApi_, LmCdl::I_VectorDataDrawingApi &drawApi_)
{
    switch (newState)
    {
    case State::CanGetFlightPath:
        missionBoundMenuItem_.setVisible(true);
        submitMissionMenuItem_.setVisible(true);
        submitMissionMenuItem_.setBackgroundColor(Qt::blue);
        submitMissionMenuItem_.setDescription("Get Flight Path");
        submitMissionMenuItem_.setIcon(":/MissionPlanning/PathIcon");
        drawing_->clearFlightPath(mission_, missionApi_);
        break;
    case State::CannotGetFlightPath:
        missionBoundMenuItem_.setVisible(true);
        submitMissionMenuItem_.setVisible(false);
        drawing_->clearFlightPath(mission_, missionApi_);
        break;
    case State::CanRunMission:
        missionBoundMenuItem_.setVisible(true);
        submitMissionMenuItem_.setVisible(true);
        submitMissionMenuItem_.setBackgroundColor(Qt::green);
        submitMissionMenuItem_.setDescription("Begin Mission");
        submitMissionMenuItem_.setIcon(":/MissionPlanning/MissionIcon");
        drawing_->clearMissionArea(drawApi_);
        break;
    case State::CanCancelMission:
        missionBoundMenuItem_.setVisible(false);
        submitMissionMenuItem_.setVisible(true);
        submitMissionMenuItem_.setBackgroundColor(Qt::red);
        submitMissionMenuItem_.setDescription("Cancel Mission");
        submitMissionMenuItem_.setIcon(":/MissionPlanning/CancelIcon");
        drawing_->clearMissionArea(drawApi_);
        break;
    default:
        missionBoundMenuItem_.setVisible(true);
        submitMissionMenuItem_.setVisible(true);
        submitMissionMenuItem_.setBackgroundColor(Qt::blue);
        submitMissionMenuItem_.setDescription("Get Flight Path");
        submitMissionMenuItem_.setIcon(":/MissionPlanning/PathIcon");
        drawing_->clearFlightPath(mission_, missionApi_);
        break;
    };
}

void UIHandler::updateUIState(const State &newState, State &m_state, LmCdl::I_ContextMenuItem &missionBoundMenuItem_, LmCdl::I_ContextMenuItem &submitMissionMenuItem_, MissionDomain &mission_, LmCdl::I_MissionDrawingApi &missionApi_, LmCdl::I_VectorDataDrawingApi &drawApi_)
{
    if (newState != m_state)
    {
        m_state = newState;
        changeUI(m_state, missionBoundMenuItem_, submitMissionMenuItem_, mission_, missionApi_, drawApi_);
    }
}