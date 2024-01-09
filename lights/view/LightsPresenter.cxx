/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsPresenter.h>

#include <I_LightsStateModel.h>

LightsPresenter::LightsPresenter(I_LightsStateModel& model, QObject* parent)
    : QObject(parent)
    , model_(model)
{
}

I_SingleLightModel* LightsPresenter::navLight() { return &model_.lightModel(StanagLightType::Nav); }

I_SingleLightModel* LightsPresenter::navIrLight() { return &model_.lightModel(StanagLightType::NavIr); }

I_SingleLightModel* LightsPresenter::strobeLight() { return &model_.lightModel(StanagLightType::Strobe); }

I_SingleLightModel* LightsPresenter::strobeIrLight() { return &model_.lightModel(StanagLightType::StrobeIr); }

I_SingleLightModel* LightsPresenter::nvdLight() { return &model_.lightModel(StanagLightType::Nvd); }

I_SingleLightModel* LightsPresenter::landingLight() { return &model_.lightModel(StanagLightType::Landing); }

I_SingleLightModel* LightsPresenter::landingIrLight() { return &model_.lightModel(StanagLightType::LandingIr); }

I_SingleLightModel* LightsPresenter::reservedLight1() { return &model_.lightModel(StanagLightType::Reserved1); }

I_SingleLightModel* LightsPresenter::vehicleSpecificLight1()
{
    return &model_.lightModel(StanagLightType::VehicleSpecific1);
}

I_SingleLightModel* LightsPresenter::vehicleSpecificLight2()
{
    return &model_.lightModel(StanagLightType::VehicleSpecific2);
}

I_SingleLightModel* LightsPresenter::vehicleSpecificLight3()
{
    return &model_.lightModel(StanagLightType::VehicleSpecific3);
}

I_SingleLightModel* LightsPresenter::vehicleSpecificLight4()
{
    return &model_.lightModel(StanagLightType::VehicleSpecific4);
}

I_SingleLightModel* LightsPresenter::vehicleSpecificLight5()
{
    return &model_.lightModel(StanagLightType::VehicleSpecific5);
}

I_SingleLightModel* LightsPresenter::vehicleSpecificLight6()
{
    return &model_.lightModel(StanagLightType::VehicleSpecific6);
}

I_SingleLightModel* LightsPresenter::vehicleSpecificLight7()
{
    return &model_.lightModel(StanagLightType::VehicleSpecific7);
}

I_SingleLightModel* LightsPresenter::vehicleSpecificLight8()
{
    return &model_.lightModel(StanagLightType::VehicleSpecific8);
}
