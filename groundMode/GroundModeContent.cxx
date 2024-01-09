/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <GroundModeContent.h>

#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleFlightMode.h>
#include <LmCdl/I_VehicleSafetyConfiguration.h>
#include <QSet>

GroundModeContent::GroundModeContent(LmCdl::I_Vehicle& vehicle)
    : vehicle_(vehicle)
{
    connect(&vehicle.flightMode(), &LmCdl::I_VehicleFlightMode::reportedFlightModeChanged, this,
        &GroundModeContent::enableOrDisableSafetyNotifications);
    enableOrDisableSafetyNotifications();
}

GroundModeContent::~GroundModeContent() { }

void GroundModeContent::enableOrDisableSafetyNotifications()
{
    LmCdl::I_VehicleSafetyConfiguration& safetyConfiguration = vehicle_.safetyConfiguration();

    if (isVehicleInGroundFlightMode()) {
        safetyConfiguration.disablePredictedTerrainClearanceSafetyCheck();
        safetyConfiguration.disableSpeedConfigurationSafetyCheck();
    } else {
        safetyConfiguration.enablePredictedTerrainClearanceSafetyCheck();
        safetyConfiguration.enableSpeedConfigurationSafetyCheck();
    }
}

bool GroundModeContent::isVehicleInGroundFlightMode()
{
    QSet<QString> groundFlightModes = {
        "Autoland",
        "Land Now",
        "Launch",
        "None",
        "Not supported",
        "Taxi",
        "Wave-off",
    };
    return groundFlightModes.contains(vehicle_.flightMode().reportedFlightMode());
}
