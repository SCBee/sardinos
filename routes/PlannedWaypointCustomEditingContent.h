/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QHash>
#include <QObject>

class PlannedWaypointCloakingDeviceMutator;

namespace LmCdl {
class I_RouteApi;
}

class PlannedWaypointCustomEditingContent : public QObject {
public:
    explicit PlannedWaypointCustomEditingContent(LmCdl::I_RouteApi& routeApi);
    ~PlannedWaypointCustomEditingContent();

private:
    void addWaypointUi(LmCdl::UniqueIdentifier waypointId);
    void removeWaypointUi(LmCdl::UniqueIdentifier waypointId);

private:
    LmCdl::I_RouteApi& routeApi_;
    QHash<LmCdl::UniqueIdentifier, PlannedWaypointCloakingDeviceMutator*> waypointUiHash_;
};
