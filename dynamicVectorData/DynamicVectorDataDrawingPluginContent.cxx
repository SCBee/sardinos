/**
\file
Copyright (c) 2021 Lockheed Martin Corp. All rights reserved.
*/

#include <DynamicVectorDataDrawingPluginContent.h>
#include <DynamicVectorDataDrawingPluginContent.moc>

#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <vcstk/core/CdlMakeUnique.h>
#include <vcstk/core/QtObjectUtility.h>

DynamicVectorDataDrawingPluginContent::DynamicVectorDataDrawingPluginContent(
    LmCdl::I_VectorDataDrawingApi& vectorDataDrawingApi, LmCdl::I_VehicleCollectionApi& vehicleCollectionApi)
    : vectorDataDrawingApi_(vectorDataDrawingApi)
    , vehicleCollectionApi_(vehicleCollectionApi)
{
    vcsQtConnect(&vehicleCollectionApi, SIGNAL(vehicleAdded(LmCdl::UniqueIdentifier)), this,
        SLOT(addDrawingForVehicle(LmCdl::UniqueIdentifier)));
    vcsQtConnect(&vehicleCollectionApi_, SIGNAL(vehicleAboutToBeRemoved(LmCdl::UniqueIdentifier)), this,
        SLOT(removeVehicle(LmCdl::UniqueIdentifier)));
}

DynamicVectorDataDrawingPluginContent::~DynamicVectorDataDrawingPluginContent() { }

void DynamicVectorDataDrawingPluginContent::addDrawingForVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    LmCdl::I_Vehicle& vehicle = vehicleCollectionApi_.vehicle(vehicleId);
    dynamicDrawings_.emplace(vehicleId, cdl::make_unique<DynamicVectorDataDrawing>(vehicle));
    auto vehicleDrawing = dynamicDrawings_.find(vehicleId);
    vectorDataDrawingApi_.addDrawingForVectorData(
        *vehicleDrawing->second, LmCdl::I_VectorDataDrawingApi::DrawingMode::OptimizedForFrequentChanges);
}

void DynamicVectorDataDrawingPluginContent::removeVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    auto vehicleDrawing = dynamicDrawings_.find(vehicleId);
    vectorDataDrawingApi_.removeDrawingForVectorData(*vehicleDrawing->second);
    dynamicDrawings_.erase(vehicleId);
}
