#include <QLabel>
#include <QTimer>

#include "Notifications.h"

#include <LmCdl/I_UserNotification.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <qobject.h>

void Notifications::notify(const std::string& msg,
                           LmCdl::I_VcsiUserNotificationApi& notApi,
                           Severity severity,
                           int durationMs)
{
    auto label = new QLabel(QString(msg.c_str()));

    label->setStyleSheet("color: black;");

    LmCdl::I_UserNotification* noti;

    switch (severity) {
        case Severity::Message:
            noti = &notApi.addNotification(label);
            noti->setBackgroundColor(Qt::white);
            break;
        case Severity::Continue:
            noti = &notApi.addNotification(label);
            noti->setBackgroundColor(Qt::green);
            break;
        case Severity::Warning:
            noti = &notApi.addNotification(label);
            noti->setBackgroundColor(Qt::yellow);
            break;
        case Severity::Danger:
            noti = &notApi.addNotification(label);
            noti->setBackgroundColor(Qt::red);
            break;
    }

    QTimer::singleShot(durationMs,
                       [&notApi, noti]() { notApi.removeNotification(*noti); });
}