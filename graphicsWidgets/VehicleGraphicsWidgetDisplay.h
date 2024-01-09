/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QtGlobal>

namespace LmCdl {
class I_GraphicsWidget;
class I_Vehicle;
class I_ContextMenuItem;
}

class VehicleGraphicsWidgetDisplay : public QObject {
    Q_OBJECT
public:
    VehicleGraphicsWidgetDisplay(LmCdl::I_Vehicle& vehicle, LmCdl::I_GraphicsWidget& graphicsWidget,
        const QPoint& offset, LmCdl::I_ContextMenuItem& contextMenuItem);
    virtual ~VehicleGraphicsWidgetDisplay();

private slots:
    void toggleWidgetVisibility();

private:
    void refreshGraphicsWidgetLocation();
    Q_DISABLE_COPY(VehicleGraphicsWidgetDisplay)

private:
    LmCdl::I_Vehicle& vehicle_;
    LmCdl::I_GraphicsWidget& graphicsWidget_;
};
