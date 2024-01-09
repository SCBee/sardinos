/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <GraphicsWidgetContentCreator.h>

#include <GdtContent.h>
#include <LmCdl/I_VehicleCollectionApi.h>

GraphicsWidgetContentCreator::GraphicsWidgetContentCreator(LmCdl::I_VehicleCollectionApi& vehicleApi,
    LmCdl::I_GdtApi& gdtApi, LmCdl::I_VcsiMapExtensionApi& mapApi,
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::I_QmlApi& qmlApi)
    : vehicleApi_(vehicleApi)
    , gdtApi_(gdtApi)
    , mapApi_(mapApi)
    , vehicleExtensionApi_(vehicleExtensionApi)
    , qmlGdtView_(qmlApi, gdtApi, mapApi)
{
    connect(&vehicleApi, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this,
        &GraphicsWidgetContentCreator::addGraphicsWidget);
    connect(&vehicleApi, &LmCdl::I_VehicleCollectionApi::vehicleAboutToBeRemoved, this,
        &GraphicsWidgetContentCreator::removeGraphicsWidget);
}

GraphicsWidgetContentCreator::~GraphicsWidgetContentCreator() { }

void GraphicsWidgetContentCreator::addGraphicsWidget(LmCdl::UniqueIdentifier vehicle)
{
    gdtContent_.emplace(vehicle,
        std::unique_ptr<GdtContent>(
            new GdtContent(vehicle, gdtApi_, vehicleApi_.vehicle(vehicle), mapApi_, vehicleExtensionApi_)));
}

void GraphicsWidgetContentCreator::removeGraphicsWidget(LmCdl::UniqueIdentifier vehicle) { gdtContent_.erase(vehicle); }
