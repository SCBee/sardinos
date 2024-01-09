/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <PlannedWaypointCustomEditingContent.h>

#include <LmCdl/I_PlannedRouteCollection.h>
#include <LmCdl/I_RouteApi.h>
#include <LmCdl/StanagRoute.h>
#include <PlannedWaypointCloakingDeviceMutator.h>
#include <PlannedWaypointCustomEditingUi.h>
#include <QList>

PlannedWaypointCustomEditingContent::PlannedWaypointCustomEditingContent(LmCdl::I_RouteApi& routeApi)
    : routeApi_(routeApi)
{
    connect(&routeApi.plannedRoutes(), &LmCdl::I_PlannedRouteCollection::waypointAdded, this,
        &PlannedWaypointCustomEditingContent::addWaypointUi);
    connect(&routeApi.plannedRoutes(), &LmCdl::I_PlannedRouteCollection::waypointRemoved, this,
        &PlannedWaypointCustomEditingContent::removeWaypointUi);
}

PlannedWaypointCustomEditingContent::~PlannedWaypointCustomEditingContent()
{
    foreach (LmCdl::UniqueIdentifier key, waypointUiHash_.keys()) {
        removeWaypointUi(key);
    }
}

void PlannedWaypointCustomEditingContent::addWaypointUi(LmCdl::UniqueIdentifier waypointId)
{
    PlannedWaypointCustomEditingUi* ui = new PlannedWaypointCustomEditingUi();
    PlannedWaypointCloakingDeviceMutator* cloakingDeviceMutator = new PlannedWaypointCloakingDeviceMutator(
        waypointId, *ui, routeApi_.plannedRoutes(), routeApi_.plannedRoutes().waypointEditor());
    waypointUiHash_.insert(waypointId, cloakingDeviceMutator);
    routeApi_.plannedRoutes().addWaypointWidget(waypointId, ui);
}

void PlannedWaypointCustomEditingContent::removeWaypointUi(LmCdl::UniqueIdentifier waypointId)
{
    PlannedWaypointCloakingDeviceMutator* cloakingDeviceMutator = waypointUiHash_.take(waypointId);
    delete cloakingDeviceMutator;
}
