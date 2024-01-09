#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/I_Vehicle.h>
#include <Message33264.h>
#include <QObject>

class StanagE2PrivateMessageControlWidgets;
class QLabel;

class StanagE2PrivateMessagePolarityReceiver : public QObject {
    Q_OBJECT

public:
    StanagE2PrivateMessagePolarityReceiver(LmCdl::I_Vehicle& vehicle, StanagE2PrivateMessageControlWidgets& widgets);

private slots:
    void processReceivedData(const LmCdl::StanagMessageType& type, const QByteArray& contents);

    QString polarityToString(LmCdl::Message33264::DeflectorPolarity polarity) const;

private:
    QLabel& reportedPolarity_;
};
