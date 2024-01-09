/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <GdtContent.h>

#include <LmCdl/ContextMenuItemGrouping.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_GraphicsWidget.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/I_Vehicle.h>
#include <QPoint>

GdtContent::GdtContent(LmCdl::UniqueIdentifier vehicleId, LmCdl::I_GdtApi& gdtApi, LmCdl::I_Vehicle& vehicle,
    LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_VcsiVehicleExtensionApi& vehicleApi)
    : widgets_(vehicleId, vehicle.reportedLocation(), gdtApi)
    , graphicsWidget_(mapApi.addGraphicsWidget(&widgets_.graphicsWidgetContents()))
    , contextMenuItem_(vehicleApi.vehicleContextMenu(vehicleId).registerMenuItem())
    , display_(vehicle, graphicsWidget_, QPoint(0, -200), contextMenuItem_)
    , mapApi_(mapApi)
{
    contextMenuItem_.setDescription("GDT Overlay");
    contextMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    contextMenuItem_.setIcon(":/gdtoverlay/GdtOverlayIcon");
}

GdtContent::~GdtContent()
{
    graphicsWidget_.setVisible(false);
    mapApi_.removeGraphicsWidget(graphicsWidget_);
}
