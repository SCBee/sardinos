/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsViewContainer.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>

LightsViewContainer::LightsViewContainer(LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiApplicationApi& applicationApi,
    LmCdl::I_QmlApi& qmlApi, I_LightsStateModel& stateModel)
    : panelPresenter_(stateModel)
    , panelView_(applicationApi.widgetExtensionApi().vehicleApi(), qmlApi, vehicleId, panelPresenter_)
{
}

LightsViewContainer::~LightsViewContainer() { }
