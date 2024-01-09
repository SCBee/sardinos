/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#include <ActionExampleDockWidget.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiDockableExtensionApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>

ActionExampleDockWidget::ActionExampleDockWidget(LmCdl::I_VcsiApplicationApi& applicationApi)
{
    setWindowTitle("Current Registered Actions");
    setObjectName(windowTitle());
    setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable | QDockWidget::DockWidgetFeature::DockWidgetFloatable
        | QDockWidget::DockWidgetFeature::DockWidgetClosable);
    setAllowedAreas(Qt::DockWidgetArea::BottomDockWidgetArea);

    applicationApi.widgetExtensionApi().dockableApi().addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, this);
}

ActionExampleDockWidget::~ActionExampleDockWidget() { }