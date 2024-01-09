#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <memory>
#include <unordered_map>

class SingleVehicleStatusDisplay;

namespace LmCdl {
class I_UnitPreferencesApi;
class I_VcsiVehicleExtensionApi;
class I_VehicleCollectionApi;
}

class VehicleStatusContent : public QObject {
public:
    VehicleStatusContent(LmCdl::I_VehicleCollectionApi& vehicleCollectionApi,
        LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::I_UnitPreferencesApi& unitPreferencesApi);
    ~VehicleStatusContent();

private:
    void addNewVehicle(LmCdl::UniqueIdentifier vehicleId);
    void removeVehicle(LmCdl::UniqueIdentifier vehicleId);

private:
    LmCdl::I_VehicleCollectionApi& vehicleCollectionApi_;
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi_;
    LmCdl::I_UnitPreferencesApi& unitPreferencesApi_;

    std::unordered_map<LmCdl::UniqueIdentifier, std::unique_ptr<SingleVehicleStatusDisplay>> vehicleDisplays_;
};
