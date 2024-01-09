/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LightsPanelView.h>
#include <LightsPresenter.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QObject>

namespace LmCdl {
class I_QmlApi;
class I_VcsiApplicationApi;
}

class I_LightsStateModel;

class LightsViewContainer : public QObject {
public:
    explicit LightsViewContainer(LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiApplicationApi& applicationApi,
        LmCdl::I_QmlApi& qmlApi, I_LightsStateModel& stateModel);
    virtual ~LightsViewContainer();

private:
    LightsPresenter panelPresenter_;
    LightsPanelView panelView_;
};
