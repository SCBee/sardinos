/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#include <PreflightChecklistExampleContent.h>

#include <LmCdl/I_PreflightChecklistApi.h>
#include <PreflightChecklistExampleBusinessLogic.h>
#include <PreflightChecklistExampleView.h>

namespace {
const bool PUT_MANUAL_STEP_FIRST = true;
const bool PUT_AUTOMATIC_STEP_FIRST = false;
}

PreflightChecklistExampleContent::PreflightChecklistExampleContent(
    LmCdl::I_PreflightChecklistApi& preflightChecklistApi,
    LmCdl::I_PreflightChecklistVisualizationApi& preflightChecklistVisualizationApi)
    : preflightChecklistApi_(preflightChecklistApi)
    , preflightChecklistVisualizationApi_(preflightChecklistVisualizationApi)
    , businessLogic1_(nullptr)
    , view1_(nullptr)
{
    connect(&preflightChecklistApi, &LmCdl::I_PreflightChecklistApi::vehiclePreflightChecklistCreated, this,
        &PreflightChecklistExampleContent::instantiateVehicleChecklistBusinessLogicAndView);
}

PreflightChecklistExampleContent::~PreflightChecklistExampleContent() { }

void PreflightChecklistExampleContent::instantiateVehicleChecklistBusinessLogicAndView(
    LmCdl::UniqueIdentifier vehicleIdentification)
{
    if (!businessLogic1_ && !view1_) {
        LmCdl::I_VehiclePreflightChecklist& vehiclePreflightChecklist
            = preflightChecklistApi_.vehiclePreflightChecklist(vehicleIdentification);

        businessLogic1_.reset(
            new PreflightChecklistExampleBusinessLogic(vehiclePreflightChecklist, PUT_MANUAL_STEP_FIRST));
        view1_.reset(new PreflightChecklistExampleView(*businessLogic1_, preflightChecklistVisualizationApi_));
    } else if (!businessLogic2_ && !view2_) {
        LmCdl::I_VehiclePreflightChecklist& vehiclePreflightChecklist
            = preflightChecklistApi_.vehiclePreflightChecklist(vehicleIdentification);

        businessLogic2_.reset(
            new PreflightChecklistExampleBusinessLogic(vehiclePreflightChecklist, PUT_AUTOMATIC_STEP_FIRST));
        view2_.reset(new PreflightChecklistExampleView(*businessLogic2_, preflightChecklistVisualizationApi_));
    }
}