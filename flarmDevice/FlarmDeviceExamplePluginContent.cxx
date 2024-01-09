/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <FlarmDeviceExamplePluginContent.h>

#include <LmCdl/I_FlarmDevice.h>

namespace {
QString transmitterStatusString(LmCdl::I_FlarmDevice::TransmitterStatus status)
{
    switch (status) {
    case LmCdl::I_FlarmDevice::TransmitterStatus::On:
        return "On";
    case LmCdl::I_FlarmDevice::TransmitterStatus::Off:
        return "Off";
    case LmCdl::I_FlarmDevice::TransmitterStatus::Unknown:
        return "Unknown";
    }
    return "Invalid device status";
}
}

FlarmDeviceExamplePluginContent::FlarmDeviceExamplePluginContent(LmCdl::I_FlarmDevice& flarmDevice)
    : flarmDevice_(flarmDevice)

{
    connectToApiSignals();
}

FlarmDeviceExamplePluginContent::~FlarmDeviceExamplePluginContent() { }

void FlarmDeviceExamplePluginContent::connectToApiSignals()
{
    connect(&flarmDevice_, &LmCdl::I_FlarmDevice::transmitterStatusUpdated, this,
        &FlarmDeviceExamplePluginContent::printStatusUpdate);
}

void FlarmDeviceExamplePluginContent::printStatusUpdate(const QHostAddress& device)
{
    qDebug("Flarm device status updated: device: %s transmitterStatus: %s", qPrintable(device.toString()),
        qPrintable(transmitterStatusString(flarmDevice_.transmitterStatus(device))));
}
