#pragma once

#include <string>

#include <LmCdl/I_VcsiUserNotificationApi.h>

class Notifications
{
public:
    enum Severity
    {
        Message,
        Continue,
        Warning,
        Danger
    };
    void notify(const std::string& msg,
                LmCdl::I_VcsiUserNotificationApi& notApi_,
                Severity severity = Severity::Message,
                int durationMs    = 3000);

private:
};