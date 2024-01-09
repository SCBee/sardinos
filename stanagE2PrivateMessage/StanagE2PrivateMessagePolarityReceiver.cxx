/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <StanagE2PrivateMessagePolarityReceiver.h>

#include <LmCdl/I_StanagE2MessagePolicy.h>
#include <LmCdl/I_StanagE2MessageReceiver.h>
#include <LmCdl/I_StanagE2Messaging.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleType.h>
#include <LmCdl/StanagVehicleType.h>
#include <QLabel>
#include <StanagE2PrivateMessageControlWidgets.h>

StanagE2PrivateMessagePolarityReceiver::StanagE2PrivateMessagePolarityReceiver(
    LmCdl::I_Vehicle& vehicle, StanagE2PrivateMessageControlWidgets& widgets)
    : reportedPolarity_(*widgets.reportedPolarity())
{
    connect(&vehicle.messaging().localCucsMessageReceiver(), &LmCdl::I_StanagE2MessageReceiver::messageReceived, this,
        &StanagE2PrivateMessagePolarityReceiver::processReceivedData);

    if (vehicle.type().vehicleType().type() == 100 && vehicle.type().vehicleType().subtype() == 1) {
        vehicle.messaging().messagePolicy().autoAcknowledgeMessageWhenReceived(LmCdl::Message33264().messageType());
    }
}

void StanagE2PrivateMessagePolarityReceiver::processReceivedData(
    const LmCdl::StanagMessageType& type, const QByteArray& contents)
{
    if (type == LmCdl::Message33264::staticMessageType()) {
        LmCdl::Message33264 message;
        message.parse(contents);
        reportedPolarity_.setText(polarityToString(message.polarity));
    }
}

QString StanagE2PrivateMessagePolarityReceiver::polarityToString(LmCdl::Message33264::DeflectorPolarity polarity) const
{
    switch (polarity) {
    case LmCdl::Message33264::DeflectorPolarity::Horizontal: {
        return "Horizontal";
    }
    case LmCdl::Message33264::DeflectorPolarity::Vertical: {
        return "Vertical";
    }
    case LmCdl::Message33264::DeflectorPolarity::Crossed: {
        return "Crossed";
    }
    }

    return "????";
}
