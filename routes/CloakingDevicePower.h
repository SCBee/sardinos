/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QHash>
#include <QObject>

class CloakingDevicePower : public QObject {
    Q_OBJECT
public:
    CloakingDevicePower();
    virtual ~CloakingDevicePower();

    void setCloakingDeviceOn(LmCdl::UniqueIdentifier waypoint, bool isOn);
    QString cloakingDevicePowerToString(LmCdl::UniqueIdentifier waypoint) const;

signals:
    void cloakingDevicePowerChanged(LmCdl::UniqueIdentifier waypoint);

private:
    bool isCloakingDeviceOn(LmCdl::UniqueIdentifier waypoint) const;

private:
    Q_DISABLE_COPY(CloakingDevicePower)

    QHash<LmCdl::UniqueIdentifier, bool> cloakingDevicePower_;
};
