/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QSet>
#include <StanagLightType.h>

namespace LmCdl {
class Action;
class I_ActionRegistrationApi;
}

class I_LightsStateModel;

class LightsToggleActionsRegistrar {
public:
    LightsToggleActionsRegistrar(I_LightsStateModel& stateModel, LmCdl::I_ActionRegistrationApi& actionRegistrationApi,
        LmCdl::UniqueIdentifier vehicleId);
    virtual ~LightsToggleActionsRegistrar();

private:
    Q_DISABLE_COPY(LightsToggleActionsRegistrar);

    void registerActionForLight(const QString lightName, StanagLightType lightType);

private:
    I_LightsStateModel& stateModel_;
    LmCdl::I_ActionRegistrationApi& actionRegistrationApi_;
    LmCdl::UniqueIdentifier vehicleId_;

    QSet<LmCdl::Action*> ownedActions_;
};
