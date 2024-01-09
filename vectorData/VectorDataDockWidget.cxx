/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <VectorDataDockWidget.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiDockableExtensionApi.h>
#include <LmCdl/I_VcsiMenuBarExtensionApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>

#include <QAction>
#include <QDockWidget>
#include <QPixmap>
#include <QToolButton>

VectorDataDockWidget::VectorDataDockWidget(LmCdl::I_VcsiApplicationApi& applicationApi)
{
    setWindowTitle("Vector Data Visualizer");
    setObjectName(windowTitle());
    setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable | QDockWidget::DockWidgetFeature::DockWidgetFloatable
        | QDockWidget::DockWidgetFeature::DockWidgetClosable);
    setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea);

    applicationApi.widgetExtensionApi().dockableApi().addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, this);

    QToolButton* openDialogButton = new QToolButton();
    openDialogButton->setCheckable(true);

    openDialogButton->setText(windowTitle());
    openDialogButton->setToolTip(windowTitle());
    applicationApi.widgetExtensionApi().menuBarApi().addItemToRightToolBar(openDialogButton);

    connect(openDialogButton, &QAbstractButton::clicked, this, &VectorDataDockWidget::showOrHideDock);
    connect(this->toggleViewAction(), &QAction::toggled, openDialogButton, &QAbstractButton::setChecked);
}

VectorDataDockWidget::~VectorDataDockWidget() { }

void VectorDataDockWidget::showOrHideDock(bool visible)
{
    if (visible) {
        show();
        raise();
    } else {
        hide();
    }
}
