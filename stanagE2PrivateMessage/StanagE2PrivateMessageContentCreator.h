#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <memory>
#include <unordered_map>
class StanagE2PrivateMessageContent;

namespace LmCdl {
class I_VehicleCollectionApi;
class I_VcsiVehicleExtensionApi;
}

class StanagE2PrivateMessageContentCreator : public QObject {
    Q_OBJECT

public:
    StanagE2PrivateMessageContentCreator(
        LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_VcsiVehicleExtensionApi& vehicleWidgetApi);
    ~StanagE2PrivateMessageContentCreator();

private slots:
    void makeDeflectorControlForVehicle(LmCdl::UniqueIdentifier vehicleId);
    void removeDeflectorControlForVehicle(LmCdl::UniqueIdentifier vehicleId);

private:
    LmCdl::I_VehicleCollectionApi& vehicleApi_;
    LmCdl::I_VcsiVehicleExtensionApi& vehicleWidgetApi_;

    std::unordered_map<LmCdl::UniqueIdentifier, std::unique_ptr<StanagE2PrivateMessageContent>> deflectorControls_;
};
