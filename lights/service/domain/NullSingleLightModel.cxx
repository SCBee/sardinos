/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <NullSingleLightModel.h>

NullSingleLightModel::NullSingleLightModel() { }

NullSingleLightModel::~NullSingleLightModel() { }

bool NullSingleLightModel::isCommandedOn() const { return false; }

bool NullSingleLightModel::isReportedOn() const { return false; }

bool NullSingleLightModel::isSupported() const { return false; }

bool NullSingleLightModel::isAvailable() const { return false; }

StanagLightType NullSingleLightModel::lightType() const { return StanagLightType::Reserved1; }

QString NullSingleLightModel::lightName() const { return QString(); }

void NullSingleLightModel::setCommandState(bool /*state*/) { }

I_SingleLightModel& NullSingleLightModel::instance()
{
    static NullSingleLightModel model;
    return model;
}
