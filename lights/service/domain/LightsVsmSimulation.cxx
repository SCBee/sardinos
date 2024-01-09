/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsVsmSimulation.h>

#include <LightsStateModel.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleType.h>
#include <LmCdl/StanagVehicleType.h>
#include <SingleLightVsmSimulation.h>
#include <StanagLightType.h>

LightsVsmSimulation::LightsVsmSimulation(LmCdl::I_Vehicle& vehicle, I_LightsStateModel& lightsStateModel)
    : lightsStateModel_(lightsStateModel)
{
    if (vehicle.type().vehicleType().type() > 500
        || vehicle.type().vehicleType().type() < 550) // VCSi Simulated Vehicles
    {
        addDemoForLight(StanagLightType::Nav);
        addDemoForLight(StanagLightType::NavIr);
        addDemoForLight(StanagLightType::Strobe);
        addDemoForLight(StanagLightType::StrobeIr);
        addDemoForLight(StanagLightType::Landing);
        addDemoForLight(StanagLightType::LandingIr);
        addDemoForLight(StanagLightType::Nvd);
        addDemoForLight(StanagLightType::Reserved1);
        addDemoForLight(StanagLightType::VehicleSpecific1);
        addDemoForLight(StanagLightType::VehicleSpecific2);
        addDemoForLight(StanagLightType::VehicleSpecific3);
        addDemoForLight(StanagLightType::VehicleSpecific4);
        addDemoForLight(StanagLightType::VehicleSpecific5);
        addDemoForLight(StanagLightType::VehicleSpecific6);
        addDemoForLight(StanagLightType::VehicleSpecific7);
        addDemoForLight(StanagLightType::VehicleSpecific8);
    }
}

LightsVsmSimulation::~LightsVsmSimulation()
{
    foreach (SingleLightVsmSimulation* demoSim, singleLightSimulation_) {
        delete demoSim;
    }
    singleLightSimulation_.clear();
}

void LightsVsmSimulation::addDemoForLight(StanagLightType type)
{
    singleLightSimulation_.append(
        new SingleLightVsmSimulation(lightsStateModel_.lightModel(type), lightsStateModel_.editableLightModel(type)));
}
