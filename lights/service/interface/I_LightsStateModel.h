/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <I_EditableSingleLightModel.h>
#include <I_SingleLightModel.h>

class I_LightsStateModel {
public:
    virtual ~I_LightsStateModel() { }

    virtual I_SingleLightModel& lightModel(StanagLightType type) = 0;
    virtual I_EditableSingleLightModel& editableLightModel(StanagLightType type) = 0;

    virtual void setIsCucsInControlOfVehicle(bool inControl) = 0;

protected: // signals
    virtual void anyLightCommandStateChanged() = 0;
};
