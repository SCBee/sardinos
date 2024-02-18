#include <QLabel>

#include "Notifications.h"
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_UserNotification.h>

void Notifications::notify(const std::string& msg, LmCdl::I_VcsiUserNotificationApi& notApi_, Severity severity)
{
    auto label = new QLabel(QString(msg.c_str()));

    label->setStyleSheet("color: black;");

    switch (severity) {
        case Severity::Message:
            notApi_.addNotification(label).setBackgroundColor(Qt::white);
            break;
        case Severity::Continue:
            notApi_.addNotification(label).setBackgroundColor(Qt::green);
            break;
        case Severity::Warning:
            notApi_.addNotification(label).setBackgroundColor(Qt::yellow);
            break;
        case Severity::Danger:
            notApi_.addNotification(label).setBackgroundColor(Qt::red);
            break;
    };
}