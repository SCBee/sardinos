/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsStateModel.h>

#include <NullEditableSingleLightModel.h>
#include <NullSingleLightModel.h>
#include <SingleLightModel.h>

LightsStateModel::LightsStateModel()
    : cucsInControlOfVehicle_(false)
{
    createSingleLightModelSupportedByDefault(StanagLightType::Nav, "Nav");
    createSingleLightModelSupportedByDefault(StanagLightType::NavIr, "Nav IR");
    createSingleLightModelSupportedByDefault(StanagLightType::Strobe, "Strobe");
    createSingleLightModelSupportedByDefault(StanagLightType::StrobeIr, "Strobe IR");
    createSingleLightModelSupportedByDefault(StanagLightType::Nvd, "NVD");
    createSingleLightModelSupportedByDefault(StanagLightType::Landing, "Land");
    createSingleLightModelSupportedByDefault(StanagLightType::LandingIr, "Land IR");

    createSingleLightModelNotSupportedByDefault(StanagLightType::Reserved1);
    createSingleLightModelNotSupportedByDefault(StanagLightType::VehicleSpecific1);
    createSingleLightModelNotSupportedByDefault(StanagLightType::VehicleSpecific2);
    createSingleLightModelNotSupportedByDefault(StanagLightType::VehicleSpecific3);
    createSingleLightModelNotSupportedByDefault(StanagLightType::VehicleSpecific4);
    createSingleLightModelNotSupportedByDefault(StanagLightType::VehicleSpecific5);
    createSingleLightModelNotSupportedByDefault(StanagLightType::VehicleSpecific6);
    createSingleLightModelNotSupportedByDefault(StanagLightType::VehicleSpecific7);
    createSingleLightModelNotSupportedByDefault(StanagLightType::VehicleSpecific8);
}

LightsStateModel::~LightsStateModel() { }

I_SingleLightModel& LightsStateModel::lightModel(StanagLightType type)
{
    if (lights_.contains(type)) {
        return *lights_[type];
    }
    return NullSingleLightModel::instance();
}

I_EditableSingleLightModel& LightsStateModel::editableLightModel(StanagLightType type)
{
    if (lights_.contains(type)) {
        return *lights_[type];
    }
    return NullEditableSingleLightModel::instance();
}

void LightsStateModel::setIsCucsInControlOfVehicle(bool inControl) { cucsInControlOfVehicle_ = inControl; }

void LightsStateModel::createSingleLightModelSupportedByDefault(StanagLightType type, const QString& name)
{
    SingleLightModel* light = new SingleLightModel(type);
    light->setSupported(true);
    light->setLightName(name);
    lights_[type] = light;
    connect(light, SIGNAL(commandChanged(bool)), this, SIGNAL(anyLightCommandStateChanged()));
}

void LightsStateModel::createSingleLightModelNotSupportedByDefault(StanagLightType type)
{
    SingleLightModel* light = new SingleLightModel(type);
    light->setSupported(false);
    light->setAvailability(false);
    lights_[type] = light;
    connect(light, SIGNAL(commandChanged(bool)), this, SIGNAL(anyLightCommandStateChanged()));
}
