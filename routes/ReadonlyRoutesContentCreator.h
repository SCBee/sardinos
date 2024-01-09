#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <memory>
#include <vector>
class ReadonlyRoutesContent;

namespace LmCdl {
class I_VehicleCollectionApi;
class I_VcsiVehicleExtensionApi;
class I_RouteApi;
}

class ReadonlyRoutesContentCreator : public QObject {
    Q_OBJECT

public:
    ReadonlyRoutesContentCreator(LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_RouteApi& routeApi,
        LmCdl::I_VcsiVehicleExtensionApi& vehicleWidgetApi);
    ~ReadonlyRoutesContentCreator();

private slots:
    void makeVehicleContent(LmCdl::UniqueIdentifier vehicleId);

private:
    LmCdl::I_VehicleCollectionApi& vehicleApi_;
    LmCdl::I_RouteApi& routeApi_;
    LmCdl::I_VcsiVehicleExtensionApi& vehicleWidgetApi_;

    std::vector<std::unique_ptr<ReadonlyRoutesContent>> downloadedRoutes_;
};
