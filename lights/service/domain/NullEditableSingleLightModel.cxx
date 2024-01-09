/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <NullEditableSingleLightModel.h>

NullEditableSingleLightModel::NullEditableSingleLightModel() { }

NullEditableSingleLightModel::~NullEditableSingleLightModel() { }

void NullEditableSingleLightModel::setReportedState(bool /*reportedState*/) { }

void NullEditableSingleLightModel::setAvailability(bool /*availability*/) { }

I_EditableSingleLightModel& NullEditableSingleLightModel::instance()
{
    static NullEditableSingleLightModel model;
    return model;
}

void NullEditableSingleLightModel::setSupported(bool /*supported*/) { }

void NullEditableSingleLightModel::setLightName(const QString& /*name*/) { }
