#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/UniqueIdentifier.h>
#include <QHash>
#include <QList>
#include <QObject>

class QLabel;

namespace LmCdl {
class I_QmlApi;
class I_VehicleCollectionApi;
class I_VcsiVehicleExtensionApi;
class Percentage;
}

class BatteryContent : public QObject {
public:
    BatteryContent(LmCdl::I_VehicleCollectionApi& vehicleCollectionApi,
        LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::I_QmlApi& qmlApi);

private:
    void addNewVehicle(LmCdl::UniqueIdentifier vehicleId);
    void removeVehicle(LmCdl::UniqueIdentifier vehicleId);

    LmCdl::Percentage vehicleFuelLevel(LmCdl::UniqueIdentifier vehicleId);
    void updateBatteryPercentage(LmCdl::UniqueIdentifier vehicleId);

private:
    LmCdl::I_VehicleCollectionApi& vehicleCollectionApi_;
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi_;
    LmCdl::I_QmlApi& qmlApi_;

    QList<LmCdl::UniqueIdentifier> vehicles_;
    QHash<LmCdl::UniqueIdentifier, QObject*> vehicleGauges_;
};
