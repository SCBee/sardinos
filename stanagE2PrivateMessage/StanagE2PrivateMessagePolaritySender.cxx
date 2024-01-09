/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <StanagE2PrivateMessagePolaritySender.h>

#include <LmCdl/I_StanagE2MessagePolicy.h>
#include <LmCdl/I_StanagE2MessageSender.h>
#include <LmCdl/StanagMessageType.h>
#include <QPushButton>
#include <StanagE2PrivateMessageControlWidgets.h>

namespace {
const int THIRTY_SECONDS = 30000;
}

StanagE2PrivateMessagePolaritySender::StanagE2PrivateMessagePolaritySender(
    StanagE2PrivateMessageControlWidgets& widgets, LmCdl::I_StanagE2MessageSender& messageSender,
    LmCdl::I_StanagE2MessagePolicy& messagePolicy)
    : widgets_(widgets)
    , messageSender_(messageSender)
{
    connect(widgets.horizontalPolarity(), &QPushButton::clicked, this,
        &StanagE2PrivateMessagePolaritySender::sendHorizontalPolarity);
    connect(widgets.verticalPolarity(), &QPushButton::clicked, this,
        &StanagE2PrivateMessagePolaritySender::sendVerticalPolarity);
    connect(widgets.crossedPolarity(), &QPushButton::clicked, this,
        &StanagE2PrivateMessagePolaritySender::sendCrossedPolarity);

    connect(&messageSender, &LmCdl::I_StanagE2MessageSender::messageAcknowledged, this,
        &StanagE2PrivateMessagePolaritySender::enableButtonsIfMessageAcknowledged);

    connect(&messageAcknowledgementGiveUpTimer_, &QTimer::timeout, this,
        &StanagE2PrivateMessagePolaritySender::giveUpOnSendingMessage);

    messagePolicy.setMessageReliable(LmCdl::Message30264::staticMessageType());
}

void StanagE2PrivateMessagePolaritySender::sendHorizontalPolarity()
{
    sendPolarity(LmCdl::Message30264::DeflectorPolarity::Horizontal);
}

void StanagE2PrivateMessagePolaritySender::sendVerticalPolarity()
{
    sendPolarity(LmCdl::Message30264::DeflectorPolarity::Vertical);
}

void StanagE2PrivateMessagePolaritySender::sendCrossedPolarity()
{
    sendPolarity(LmCdl::Message30264::DeflectorPolarity::Crossed);
}

void StanagE2PrivateMessagePolaritySender::sendPolarity(LmCdl::Message30264::DeflectorPolarity polarity)
{
    LmCdl::Message30264 deflectorCommand;
    deflectorCommand.polarity = polarity;
    disableAllButtons();
    messageRequiringAcknowledgement_ = messageSender_.send(deflectorCommand);
    messageAcknowledgementGiveUpTimer_.start(THIRTY_SECONDS);
}

void StanagE2PrivateMessagePolaritySender::enableButtonsIfMessageAcknowledged(LmCdl::UniqueIdentifier instanceId)
{
    if (instanceId == messageRequiringAcknowledgement_) {
        messageAcknowledgementGiveUpTimer_.stop();
        enableAllButtons();
    }
}

void StanagE2PrivateMessagePolaritySender::giveUpOnSendingMessage()
{
    messageSender_.stopSending(messageRequiringAcknowledgement_);
    enableAllButtons();
}

void StanagE2PrivateMessagePolaritySender::enableAllButtons()
{
    widgets_.horizontalPolarity()->setEnabled(true);
    widgets_.verticalPolarity()->setEnabled(true);
    widgets_.crossedPolarity()->setEnabled(true);
}

void StanagE2PrivateMessagePolaritySender::disableAllButtons()
{
    widgets_.horizontalPolarity()->setEnabled(false);
    widgets_.verticalPolarity()->setEnabled(false);
    widgets_.crossedPolarity()->setEnabled(false);
}
