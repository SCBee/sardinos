/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <NotificationWithLongString.h>

#include <LmCdl/ApplicationColors.h>
#include <LmCdl/I_UserNotification.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleVelocity.h>
#include <LmCdl/Speed.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

NotificationWithLongString::NotificationWithLongString(
    LmCdl::UniqueIdentifier vehicleId, LmCdl::I_Vehicle& vehicle, LmCdl::I_VcsiUserNotificationApi& userNotificationApi)
    : vehicle_(vehicle)
    , userNotificationApi_(userNotificationApi)

    , notificationContents_(new QWidget())

    , airspeed_(new QLabel(notificationContents_.data()))
    , labelWithALotOfText_(new QLabel(notificationContents_.data()))

    , notification_(userNotificationApi.addNotificationForVehicle(vehicleId, notificationContents_))
{
    QVBoxLayout* layout = new QVBoxLayout(notificationContents_.data());

    layout->setSpacing(0);
    layout->setMargin(0);

    layout->addWidget(airspeed_);
    layout->addWidget(labelWithALotOfText_);

    airspeed_->setObjectName("warningTextLabel");
    labelWithALotOfText_->setObjectName("warningTextLabel");

    labelWithALotOfText_->setText("When a warning has too much text to fit within the warning dimensions, then "
                                  "setWordWrap(true) must be called on the label in order to wrap the text.");
    labelWithALotOfText_->setWordWrap(true);

    connect(&vehicle_.reportedVelocity(), &LmCdl::I_VehicleVelocity::indicatedAirspeedChanged, this,
        &NotificationWithLongString::refreshContents);
    refreshContents();

    notification_.setClearable(true);
    notification_.setBackgroundColor(LmCdl::ApplicationColors::advisory());
}

NotificationWithLongString::~NotificationWithLongString() { userNotificationApi_.removeNotification(notification_); }

void NotificationWithLongString::refreshContents()
{
    airspeed_->setText(
        QString("Airspeed: %1kts").arg(vehicle_.reportedVelocity().indicatedAirspeed().value(LmCdl::SpeedUnit::Knots)));
}
