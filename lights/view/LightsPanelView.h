/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>

namespace LmCdl {
class I_QmlApi;
class I_VcsiVehicleExtensionApi;
class UniqueIdentifier;
}
class LightsPresenter;

class LightsPanelView : public QObject {
public:
    explicit LightsPanelView(LmCdl::I_VcsiVehicleExtensionApi& vehiclePanelWidgetApi, LmCdl::I_QmlApi& qmlApi,
        LmCdl::UniqueIdentifier vehicleId, LightsPresenter& presenter);
    virtual ~LightsPanelView();

private:
    QObject* viewRootObject_;
};
