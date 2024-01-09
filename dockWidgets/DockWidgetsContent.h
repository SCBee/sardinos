/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <QObject>

class QAbstractButton;
class QDockWidget;
class QLabel;
class QWidget;

namespace LmCdl {
class I_VcsiDockableExtensionApi;
class I_VcsiVehicleExtensionApi;
class UniqueIdentifier;
class I_VcsiMenuBarExtensionApi;
}

class DockWidgetsContent : public QObject {
public:
    explicit DockWidgetsContent(LmCdl::I_VcsiDockableExtensionApi& dockableApi,
        LmCdl::I_VcsiVehicleExtensionApi& vehicleApi, LmCdl::I_VcsiMenuBarExtensionApi& menuBarApi);

private:
    void layoutContents();
    void createAndRegisterDockWidget(LmCdl::I_VcsiDockableExtensionApi& dockableApi);
    void createMenuBarButton(LmCdl::I_VcsiMenuBarExtensionApi& menuBarApi);
    void lookupCurrentWeather();
    void updateActiveVehicle(LmCdl::UniqueIdentifier vehicleId);
    void showOrHideWeatherDock(bool visible);

    QWidget* mainWidget_;
    QDockWidget* dockWidget_;

    QLabel* weatherToday_;
    QLabel* weatherTommorrow_;

    QLabel* activeVehicleId_;
    QAbstractButton* openDialogButton_;
};
