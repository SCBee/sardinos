/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QPointer>
#include <QTimer>
#include <QtGlobal>

namespace LmCdl {
class I_UserNotification;
class I_VcsiUserNotificationApi;
class UniqueIdentifier;
}

class QLabel;
class QWidget;

class NotificationToBeRemovedAfterTime : public QObject {
    Q_OBJECT
public:
    NotificationToBeRemovedAfterTime(
        LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiUserNotificationApi& userNotificationApi);
    virtual ~NotificationToBeRemovedAfterTime();

private slots:
    void updateNotificationText();
    void removeNotification();

private:
    Q_DISABLE_COPY(NotificationToBeRemovedAfterTime)

    LmCdl::I_VcsiUserNotificationApi& userNotificationApi_;

    int timeRemainingInMilliseconds_;
    QTimer updateTimer_;
    QTimer removeTimer_;
    QPointer<QLabel> notificationContents_;
    LmCdl::I_UserNotification* notification_;
};
