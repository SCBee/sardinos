#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/UniqueIdentifier.h>

namespace LmCdl {
class I_VcsiVehicleExtensionApi;
}

class QLabel;
class QPushButton;
class QWidget;

class StanagE2PrivateMessageControlWidgets {
public:
    StanagE2PrivateMessageControlWidgets(
        LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiVehicleExtensionApi& widgetApi);

    QPushButton* horizontalPolarity();
    QPushButton* verticalPolarity();
    QPushButton* crossedPolarity();

    QLabel* reportedPolarity();

private:
    void layoutWidgets();
    void registerTabWithApi(LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiVehicleExtensionApi& widgetApi);

    QWidget* mainWidget_;

    QPushButton* horizontalPolarity_;
    QPushButton* verticalPolarity_;
    QPushButton* crossedPolarity_;

    QLabel* reportedPolarityLabel_;
    QLabel* reportedPolarity_;
};
