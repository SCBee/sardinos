/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <SingleLightModel.h>

SingleLightModel::SingleLightModel(StanagLightType type)
    : isCommandedOn_(false)
    , isReportedOn_(false)
    , isAvailable_(true)
    , isSupported_(false)
    , lightType_(type)
{
}

SingleLightModel::~SingleLightModel() { }

bool SingleLightModel::isCommandedOn() const { return isCommandedOn_; }

bool SingleLightModel::isReportedOn() const { return isReportedOn_; }

bool SingleLightModel::isSupported() const { return isSupported_; }

bool SingleLightModel::isAvailable() const { return isAvailable_; }

StanagLightType SingleLightModel::lightType() const { return lightType_; }

QString SingleLightModel::lightName() const { return lightName_; }

void SingleLightModel::setCommandState(bool state)
{
    if (isCommandedOn_ != state) {
        isCommandedOn_ = state;
        emit commandChanged(isCommandedOn_);
    }
}

void SingleLightModel::setReportedState(bool reportedState)
{
    if (isReportedOn_ != reportedState) {
        isReportedOn_ = reportedState;
        emit reportChanged(isReportedOn_);
    }
}

void SingleLightModel::setSupported(bool supported)
{
    if (isSupported_ != supported) {
        isSupported_ = supported;
        emit supportChanged(isSupported_);
    }
}

void SingleLightModel::setAvailability(bool availability)
{
    if (isAvailable_ != availability) {
        isAvailable_ = availability;
        emit availabilityChanged(isAvailable_);
    }
}

void SingleLightModel::setLightName(const QString& name)
{
    if (lightName_ != name) {
        lightName_ = name;
        emit lightNameChanged(lightName_);
    }
}
