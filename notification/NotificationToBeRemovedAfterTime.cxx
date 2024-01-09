/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <NotificationToBeRemovedAfterTime.h>

#include <LmCdl/ApplicationColors.h>
#include <LmCdl/I_UserNotification.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

namespace {
const int ONE_SECOND = 1000;
}
NotificationToBeRemovedAfterTime::NotificationToBeRemovedAfterTime(
    LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiUserNotificationApi& userNotificationApi)
    : userNotificationApi_(userNotificationApi)
    , timeRemainingInMilliseconds_(20000)

    , notificationContents_(new QLabel())
{
    notificationContents_->setObjectName("warningTextLabel");
    notificationContents_->setWordWrap(true);

    notification_ = &userNotificationApi_.addNotificationForVehicle(vehicleId, notificationContents_);
    notification_->setBackgroundColor(LmCdl::ApplicationColors::caution());

    removeTimer_.setSingleShot(true);
    removeTimer_.start(timeRemainingInMilliseconds_);
    connect(&removeTimer_, &QTimer::timeout, this, &NotificationToBeRemovedAfterTime::removeNotification);

    connect(&updateTimer_, &QTimer::timeout, this, &NotificationToBeRemovedAfterTime::updateNotificationText);
    updateTimer_.start(ONE_SECOND);
    updateNotificationText();
}

NotificationToBeRemovedAfterTime::~NotificationToBeRemovedAfterTime() { removeNotification(); }

void NotificationToBeRemovedAfterTime::updateNotificationText()
{
    QString text
        = QString("This warning will be removed after %1 seconds").arg(timeRemainingInMilliseconds_ / ONE_SECOND);
    notificationContents_->setText(text);
    timeRemainingInMilliseconds_ -= ONE_SECOND;
}

void NotificationToBeRemovedAfterTime::removeNotification()
{
    updateTimer_.stop();
    if (notification_) {
        userNotificationApi_.removeNotification(*notification_);
        notification_ = NULL;
    }
}
