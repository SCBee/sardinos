/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <SingleLightVsmSimulation.h>

#include <I_EditableSingleLightModel.h>
#include <I_SingleLightModel.h>

SingleLightVsmSimulation::SingleLightVsmSimulation(I_SingleLightModel& light, I_EditableSingleLightModel& editableLight)
    : light_(light)
    , editableLight_(editableLight)
{
    connect(&timer_, SIGNAL(timeout()), this, SLOT(syncState()));
    timer_.start(2000);
}

SingleLightVsmSimulation::~SingleLightVsmSimulation() { }

void SingleLightVsmSimulation::syncState() { editableLight_.setReportedState(light_.isCommandedOn()); }
