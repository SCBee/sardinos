/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QmlGdtView.h>
#include <QtGlobal>
#include <memory>
#include <unordered_map>

class GdtContent;
namespace LmCdl {
class I_GdtApi;
class I_QmlApi;
class I_VehicleCollectionApi;
class I_VcsiMapExtensionApi;
class I_VcsiVehicleExtensionApi;
}

class GraphicsWidgetContentCreator : public QObject {
    Q_OBJECT
public:
    GraphicsWidgetContentCreator(LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_GdtApi& gdtApi,
        LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi,
        LmCdl::I_QmlApi& qmlApi);
    virtual ~GraphicsWidgetContentCreator();

private slots:
    void addGraphicsWidget(LmCdl::UniqueIdentifier vehicle);
    void removeGraphicsWidget(LmCdl::UniqueIdentifier vehicle);

private:
    Q_DISABLE_COPY(GraphicsWidgetContentCreator);

    LmCdl::I_VehicleCollectionApi& vehicleApi_;
    LmCdl::I_GdtApi& gdtApi_;
    LmCdl::I_VcsiMapExtensionApi& mapApi_;
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi_;

    std::unordered_map<LmCdl::UniqueIdentifier, std::unique_ptr<GdtContent>> gdtContent_;
    QmlGdtView qmlGdtView_;
};
