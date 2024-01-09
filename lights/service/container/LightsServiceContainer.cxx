/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsServiceContainer.h>

#include <LightsConfigurationReceiver.h>
#include <LightsControlStateSyncer.h>
#include <LightsPowerRequestSender.h>
#include <LightsStateModel.h>
#include <LightsStatusReceiver.h>
#include <LightsToggleActionsRegistrar.h>
#include <LightsVsmSimulation.h>
#include <LmCdl/I_ActionApi.h>
#include <LmCdl/I_Vehicle.h>

LightsServiceContainer::LightsServiceContainer(
    LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId, LmCdl::I_ActionApi& actionApi)
    : lightsStateModel_(new LightsStateModel())
    , lightsActionsRegistrar_(
          new LightsToggleActionsRegistrar(*lightsStateModel_, actionApi.actionRegistrationApi(), vehicleId))
    , lightsStatusReceiver_(new LightsStatusReceiver(vehicle, vehicleId, *lightsStateModel_))
    , lightsConfigurationReceiver_(new LightsConfigurationReceiver(vehicle, vehicleId, *lightsStateModel_))
    , controlStateSyncer_(new LightsControlStateSyncer(vehicle.reportedControlState(), *lightsStateModel_))
    , powerRequestSender_(new LightsPowerRequestSender(vehicle, vehicleId, *lightsStateModel_))
    , lightsVsmSimulation_(new LightsVsmSimulation(vehicle, *lightsStateModel_))
{
}

LightsServiceContainer::~LightsServiceContainer() { }

I_LightsStateModel& LightsServiceContainer::stateModel() { return *lightsStateModel_; }
