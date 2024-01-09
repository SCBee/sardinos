/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <StanagLightType.h>

class I_LightsStateModel;

namespace LmCdl {
class I_Vehicle;
class StanagMessageType;
}

class LightsConfigurationReceiver : public QObject {
    Q_OBJECT

public:
    explicit LightsConfigurationReceiver(
        LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId, I_LightsStateModel& model);
    virtual ~LightsConfigurationReceiver();

private slots:
    void processMessageData(const LmCdl::StanagMessageType& type, const QByteArray& contents);

private:
    StanagLightType typeFromMessage(int messageEnumerationIndex) const;

private:
    LmCdl::UniqueIdentifier vehicleId_;
    I_LightsStateModel& lightsStateModel_;
};
