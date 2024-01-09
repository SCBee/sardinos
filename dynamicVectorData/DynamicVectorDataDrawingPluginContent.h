/**
\file
Copyright (c) 2021 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <DynamicVectorDataDrawing.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>

namespace LmCdl {
class I_VectorDataDrawingApi;
class I_VehicleCollectionApi;
class I_Vehicle;
}

class DynamicVectorDataDrawingPluginContent : public QObject {
    Q_OBJECT
public:
    explicit DynamicVectorDataDrawingPluginContent(
        LmCdl::I_VectorDataDrawingApi& vectorDataDrawingApi, LmCdl::I_VehicleCollectionApi& vehicleCollectionApi);
    ~DynamicVectorDataDrawingPluginContent();

private slots:
    void addDrawingForVehicle(LmCdl::UniqueIdentifier vehicleId);
    void removeVehicle(LmCdl::UniqueIdentifier vehicleId);

private:
    Q_DISABLE_COPY(DynamicVectorDataDrawingPluginContent);

private:
    LmCdl::I_VectorDataDrawingApi& vectorDataDrawingApi_;
    LmCdl::I_VehicleCollectionApi& vehicleCollectionApi_;

    std::unordered_map<LmCdl::UniqueIdentifier, std::unique_ptr<DynamicVectorDataDrawing>> dynamicDrawings_;
};
