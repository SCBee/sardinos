/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <PlannedWaypointCustomEditingUi.h>

#include <ui_PlannedWaypointCustomProperties.h>

PlannedWaypointCustomEditingUi::PlannedWaypointCustomEditingUi()
    : ui_(new Ui_PlannedWaypointCustomProperties())
{
    ui_->setupUi(this);
}

PlannedWaypointCustomEditingUi::~PlannedWaypointCustomEditingUi() { }

QCheckBox& PlannedWaypointCustomEditingUi::cloakingDeviceEdit() { return *ui_->cloakingDeviceEdit; }
