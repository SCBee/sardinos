/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QScopedPointer>

class I_LightsStateModel;
class LightsConfigurationReceiver;
class LightsControlStateSyncer;
class LightsVsmSimulation;
class LightsPowerRequestSender;
class LightsStateModel;
class LightsStatusReceiver;
class LightsToggleActionsRegistrar;

namespace LmCdl {
class I_ActionApi;
class I_Vehicle;
class UniqueIdentifier;
}

class LightsServiceContainer : public QObject {
public:
    explicit LightsServiceContainer(
        LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId, LmCdl::I_ActionApi& actionApi);
    virtual ~LightsServiceContainer();

    I_LightsStateModel& stateModel();

private:
    QScopedPointer<LightsStateModel> lightsStateModel_;
    QScopedPointer<LightsToggleActionsRegistrar> lightsActionsRegistrar_;
    QScopedPointer<LightsStatusReceiver> lightsStatusReceiver_;
    QScopedPointer<LightsConfigurationReceiver> lightsConfigurationReceiver_;
    QScopedPointer<LightsControlStateSyncer> controlStateSyncer_;
    QScopedPointer<LightsPowerRequestSender> powerRequestSender_;

    // This class pretends that the vehicle has responded after a certain period
    // of time elapses
    // It is entirely used to mimic how a VSM should respond when an appropriate
    // vehicle or simulator
    // are not available (e.g. in a demo scenario).
    QScopedPointer<LightsVsmSimulation> lightsVsmSimulation_;
};
