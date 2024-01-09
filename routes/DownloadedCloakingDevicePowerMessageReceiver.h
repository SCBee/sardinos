/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
class CloakingDevicePower;
class QByteArray;
namespace LmCdl {
class I_Vehicle;
class Message33265;
class StanagMessageType;
}

class DownloadedCloakingDevicePowerMessageReceiver : public QObject {
public:
    DownloadedCloakingDevicePowerMessageReceiver(LmCdl::I_Vehicle& vehicle, CloakingDevicePower& cloakingDevicePower);
    virtual ~DownloadedCloakingDevicePowerMessageReceiver();

private:
    void processReceivedData(const LmCdl::StanagMessageType& type, const QByteArray& contents);
    void setCloakingDevicePower(const LmCdl::Message33265& message);

private:
    Q_DISABLE_COPY(DownloadedCloakingDevicePowerMessageReceiver)

    CloakingDevicePower& cloakingDevicePower_;
};
