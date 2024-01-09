/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QPointer>
#include <QtGlobal>

namespace LmCdl {
class UniqueIdentifier;
class I_Vehicle;
class I_UserNotification;
class I_VcsiUserNotificationApi;
}

class QLabel;
class QWidget;

class NotificationWithLongString : public QObject {
    Q_OBJECT
public:
    NotificationWithLongString(LmCdl::UniqueIdentifier vehicleId, LmCdl::I_Vehicle& vehicle,
        LmCdl::I_VcsiUserNotificationApi& userNotificationApi);
    virtual ~NotificationWithLongString();

private slots:
    void refreshContents();

private:
    Q_DISABLE_COPY(NotificationWithLongString)

    LmCdl::I_Vehicle& vehicle_;
    LmCdl::I_VcsiUserNotificationApi& userNotificationApi_;

    QPointer<QWidget> notificationContents_;

    QLabel* airspeed_;
    QLabel* labelWithALotOfText_;
    LmCdl::I_UserNotification& notification_;
};
