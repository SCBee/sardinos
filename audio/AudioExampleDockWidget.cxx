/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#include <AudioExampleDockWidget.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiDockableExtensionApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>

AudioExampleDockWidget::AudioExampleDockWidget(LmCdl::I_VcsiApplicationApi& applicationApi)
{
    setWindowTitle("Audio Tester");
    setObjectName(windowTitle());
    setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable | QDockWidget::DockWidgetFeature::DockWidgetFloatable
        | QDockWidget::DockWidgetFeature::DockWidgetClosable);
    setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea);

    applicationApi.widgetExtensionApi().dockableApi().addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, this);
}

AudioExampleDockWidget::~AudioExampleDockWidget() { }
