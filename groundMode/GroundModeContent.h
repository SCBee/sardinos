/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QtGlobal>

namespace LmCdl {
class I_Vehicle;
}

class GroundModeContent : public QObject {
public:
    explicit GroundModeContent(LmCdl::I_Vehicle& vehicle);
    virtual ~GroundModeContent();

private:
    void enableOrDisableSafetyNotifications();
    bool isVehicleInGroundFlightMode();

    Q_DISABLE_COPY(GroundModeContent)

private:
    LmCdl::I_Vehicle& vehicle_;
};
