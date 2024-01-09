/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>

namespace LmCdl {
class I_VehicleControlState;
}

class I_LightsStateModel;

class LightsControlStateSyncer : public QObject {
public:
    Q_OBJECT

public:
    LightsControlStateSyncer(LmCdl::I_VehicleControlState& reportedControlState, I_LightsStateModel& lightsStateModel);
    virtual ~LightsControlStateSyncer();

private slots:
    void updateControlState();

private:
    I_LightsStateModel& lightsStateModel_;
    LmCdl::I_VehicleControlState& reportedControlState_;
};
