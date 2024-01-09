#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/UniqueIdentifier.h>
#include <Message30264.h>
#include <QObject>
#include <QTimer>

namespace LmCdl {
class I_StanagE2MessageSender;
class I_StanagE2MessagePolicy;
}

class StanagE2PrivateMessageControlWidgets;

class StanagE2PrivateMessagePolaritySender : public QObject {
    Q_OBJECT

public:
    StanagE2PrivateMessagePolaritySender(StanagE2PrivateMessageControlWidgets& widgets,
        LmCdl::I_StanagE2MessageSender& messageSender, LmCdl::I_StanagE2MessagePolicy& messagePolicy);

private slots:
    void sendHorizontalPolarity();
    void sendVerticalPolarity();
    void sendCrossedPolarity();

    void enableButtonsIfMessageAcknowledged(LmCdl::UniqueIdentifier instanceId);
    void giveUpOnSendingMessage();

private:
    void sendPolarity(LmCdl::Message30264::DeflectorPolarity polarity);

    void enableAllButtons();
    void disableAllButtons();

    StanagE2PrivateMessageControlWidgets& widgets_;
    LmCdl::I_StanagE2MessageSender& messageSender_;

    LmCdl::UniqueIdentifier messageRequiringAcknowledgement_;
    QTimer messageAcknowledgementGiveUpTimer_;
};
