/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QHostAddress>
#include <QObject>
#include <QScopedPointer>

class QHostAddress;
namespace LmCdl {
class I_FlarmDevice;
}

class FlarmDeviceExamplePluginContent : public QObject {
    Q_OBJECT

public:
    explicit FlarmDeviceExamplePluginContent(LmCdl::I_FlarmDevice& flarmDevice);
    virtual ~FlarmDeviceExamplePluginContent();

private slots:
    void printStatusUpdate(const QHostAddress& device);

private:
    void connectToApiSignals();

private:
    LmCdl::I_FlarmDevice& flarmDevice_;
};
