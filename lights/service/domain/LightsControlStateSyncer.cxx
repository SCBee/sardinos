/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsControlStateSyncer.h>

#include <I_LightsStateModel.h>
#include <LmCdl/I_VehicleControlState.h>

LightsControlStateSyncer::LightsControlStateSyncer(
    LmCdl::I_VehicleControlState& reportedControlState, I_LightsStateModel& lightsStateMode)
    : lightsStateModel_(lightsStateMode)
    , reportedControlState_(reportedControlState)
{
    connect(&reportedControlState_, &LmCdl::I_VehicleControlState::controlStateChanged, this,
        &LightsControlStateSyncer::updateControlState);
}

LightsControlStateSyncer::~LightsControlStateSyncer() { }

void LightsControlStateSyncer::updateControlState()
{
    lightsStateModel_.setIsCucsInControlOfVehicle(reportedControlState_.isControlled());
}
