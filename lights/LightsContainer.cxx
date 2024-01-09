/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsContainer.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_Vehicle.h>

LightsContainer::LightsContainer(LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId,
    LmCdl::I_VcsiApplicationApi& applicationApi, LmCdl::I_QmlApi& qmlApi, LmCdl::I_ActionApi& actionApi)
    : serviceContainer_(vehicle, vehicleId, actionApi)
    , viewContainer_(vehicleId, applicationApi, qmlApi, serviceContainer_.stateModel())
{
}

LightsContainer::~LightsContainer() { }
