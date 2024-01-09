/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <GdtWidgets.h>
#include <QtGlobal>
#include <VehicleGraphicsWidgetDisplay.h>

namespace LmCdl {
class I_GdtApi;
class I_GraphicsWidget;
class I_Vehicle;
class I_VcsiMapExtensionApi;
class I_VcsiVehicleExtensionApi;
class UniqueIdentifier;
}

class GdtContent {
public:
    explicit GdtContent(LmCdl::UniqueIdentifier vehicleId, LmCdl::I_GdtApi& gdtApi, LmCdl::I_Vehicle& vehicle,
        LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_VcsiVehicleExtensionApi& vehicleApi);
    virtual ~GdtContent();

private:
    Q_DISABLE_COPY(GdtContent)

private:
    GdtWidgets widgets_;
    LmCdl::I_GraphicsWidget& graphicsWidget_;
    LmCdl::I_ContextMenuItem& contextMenuItem_;
    VehicleGraphicsWidgetDisplay display_;
    LmCdl::I_VcsiMapExtensionApi& mapApi_;
};
