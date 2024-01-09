/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <VehicleGraphicsWidgetDisplay.h>

#include <LmCdl/Distance.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_GraphicsWidget.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleLocation.h>
#include <QGeoCoordinate>
#include <QPoint>

VehicleGraphicsWidgetDisplay::VehicleGraphicsWidgetDisplay(LmCdl::I_Vehicle& vehicle,
    LmCdl::I_GraphicsWidget& graphicsWidget, const QPoint& offset, LmCdl::I_ContextMenuItem& contextMenuItem)
    : vehicle_(vehicle)
    , graphicsWidget_(graphicsWidget)
{
    graphicsWidget_.setOffsetInPixels(offset);
    graphicsWidget_.setVisible(false);
    connect(&contextMenuItem, &LmCdl::I_ContextMenuItem::clicked, this,
        &VehicleGraphicsWidgetDisplay::toggleWidgetVisibility);

    connect(&vehicle_.reportedLocation(), &LmCdl::I_VehicleLocation::locationChanged, this,
        &VehicleGraphicsWidgetDisplay::refreshGraphicsWidgetLocation);
    refreshGraphicsWidgetLocation();
}

VehicleGraphicsWidgetDisplay::~VehicleGraphicsWidgetDisplay() { }

void VehicleGraphicsWidgetDisplay::toggleWidgetVisibility()
{
    graphicsWidget_.setVisible(!graphicsWidget_.isVisible());
}

void VehicleGraphicsWidgetDisplay::refreshGraphicsWidgetLocation()
{
    graphicsWidget_.setLocation(vehicle_.reportedLocation().location());
}
