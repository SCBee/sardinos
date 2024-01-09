/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <NotificationToBeRemovedAfterTime.h>
#include <NotificationWithLongString.h>
#include <QtGlobal>

namespace LmCdl {
class I_Vehicle;
class I_VcsiUserNotificationApi;
class UniqueIdentifier;
}

class ExampleNotificationContent {
public:
    ExampleNotificationContent(LmCdl::UniqueIdentifier vehicleId, LmCdl::I_Vehicle& vehicle,
        LmCdl::I_VcsiUserNotificationApi& userNotificationApi);
    virtual ~ExampleNotificationContent();

private:
    Q_DISABLE_COPY(ExampleNotificationContent)

    NotificationWithLongString longStringNotification_;
    NotificationToBeRemovedAfterTime notificationToBeRemoved_;
};
