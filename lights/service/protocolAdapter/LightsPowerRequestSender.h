/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>

class I_LightsStateModel;

namespace LmCdl {
class I_Vehicle;
class I_StanagE2MessageSender;
}

/*
 * Responsible to monitor request signals from the LightsStateService
 * These requests will result in a messages being sent to the VSM.
 */
class LightsPowerRequestSender : public QObject {
    Q_OBJECT

public:
    explicit LightsPowerRequestSender(
        LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId, I_LightsStateModel& lightsStateModel);
    virtual ~LightsPowerRequestSender();

private slots:
    void updateLightCommandState();

private:
    int combinedLightPowerStateDemand();
    int encodedLightBit(int offset, bool isOn);

private:
    LmCdl::UniqueIdentifier vehicleId_;
    LmCdl::I_StanagE2MessageSender& messageSender_;
    I_LightsStateModel& lightsStateModel_;
};
