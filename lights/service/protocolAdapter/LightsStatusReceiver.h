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
class StanagMessageType;
}

class LightsStatusReceiver : public QObject {
    Q_OBJECT

public:
    explicit LightsStatusReceiver(
        LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId, I_LightsStateModel& model);
    virtual ~LightsStatusReceiver();

private slots:
    void processMessageData(const LmCdl::StanagMessageType& type, const QByteArray& contents);

private:
    bool isLightAtOffsetOn(int offset, int encodedField) const;

private:
    LmCdl::UniqueIdentifier vehicleId_;
    I_LightsStateModel& lightsStateModel_;
};
