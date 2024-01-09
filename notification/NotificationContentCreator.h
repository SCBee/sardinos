/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>

namespace LmCdl {
class I_VehicleCollectionApi;
class I_VcsiUserNotificationApi;
}

class ExampleNotificationContent;

class NotificationContentCreator : public QObject {
    Q_OBJECT
public:
    NotificationContentCreator(
        LmCdl::I_VehicleCollectionApi& vehicleApi, LmCdl::I_VcsiUserNotificationApi& userNotificationApi);
    virtual ~NotificationContentCreator();

private slots:
    void addWarningsForVehicle(LmCdl::UniqueIdentifier vehicle);
    void removeWarningsForVehicle(LmCdl::UniqueIdentifier vehicle);

private:
    Q_DISABLE_COPY(NotificationContentCreator)

    LmCdl::I_VehicleCollectionApi& vehicleApi_;
    LmCdl::I_VcsiUserNotificationApi& userNotificationApi_;

    std::unordered_map<LmCdl::UniqueIdentifier, std::unique_ptr<ExampleNotificationContent>> content_;
};
