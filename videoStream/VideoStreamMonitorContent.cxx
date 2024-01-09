/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#include <LmCdl/I_VideoStreamApi.h>
#include <LmCdl/I_VideoStreamApiCollection.h>
#include <LmCdl/VideoSnapshot.h>
#include <QImage>
#include <QThread>
#include <VideoStreamMonitorContent.h>

namespace {
const double MISSING_SENSOR_ALTITUDE = -9999.9;
}

VideoStreamMonitorContent::VideoStreamMonitorContent(LmCdl::I_VideoStreamApiCollection& videoStreamApiCollection)
    : videoStreamApiCollection_(videoStreamApiCollection)
    , snapshotTimer_()
{
    initializeSnapshotTimer();
    connectToApiSignals();
    registerVideoStream("FakeDescription", "FakeUri");
}

VideoStreamMonitorContent::~VideoStreamMonitorContent() { unregisterOwnedVideoStreams(); }

void VideoStreamMonitorContent::initializeSnapshotTimer()
{
    snapshotTimer_.setSingleShot(false);
    snapshotTimer_.start(5000);
    connect(&snapshotTimer_, SIGNAL(timeout()), this, SLOT(logSnapshotInfo()));
}

void VideoStreamMonitorContent::connectToApiSignals()
{
    connect(&videoStreamApiCollection_, SIGNAL(streamRegistered(LmCdl::I_VideoStreamApi&)), this,
        SLOT(addToKnownVideoStreams(LmCdl::I_VideoStreamApi&)));

    connect(&videoStreamApiCollection_, SIGNAL(streamAboutToBeUnregistered(LmCdl::I_VideoStreamApi&)), this,
        SLOT(removeFromKnownVideoStreams(LmCdl::I_VideoStreamApi&)));
}

void VideoStreamMonitorContent::addToKnownVideoStreams(LmCdl::I_VideoStreamApi& videoStreamApi)
{
    allKnownVideoStreams_.append(&videoStreamApi);
}

void VideoStreamMonitorContent::removeFromKnownVideoStreams(LmCdl::I_VideoStreamApi& videoStreamApi)
{
    allKnownVideoStreams_.removeAll(&videoStreamApi);
    // we need to remove from owned video streams as well, or else if the stream is removed on a lower level,
    // ownedVideoStreams will have a dangling pointer, and cause a crash when
    // this class calls its own unregister stream. This is because VideoStreamMonitorContent::registerVideoStream
    // registers the stream on a lower level. one other solution is to have an if guard in
    // VideoStreamMonitorContent::unregisterOwnedVideoStreams to check if the owned video stream exists as a known video
    // stream, allowing this example to still keep track of what it "owns"...
    ownedVideoStreams_.removeAll(&videoStreamApi);
}

void VideoStreamMonitorContent::logSnapshotInfo()
{
    for (int i = 0; i < allKnownVideoStreams_.count(); ++i) {
        LmCdl::VideoSnapshot videoSnapshot = allKnownVideoStreams_.at(i)->getVideoSnapshot();
        QImage frameImage = videoSnapshot.videoFrame();
        QHash<LmCdl::VideoKlv601Key, QVariant> klvData = videoSnapshot.klvData();

        qDebug("Frame info for stream %d:", i);
        qDebug("   Image size: %d x %d", frameImage.width(), frameImage.height());
        qDebug("   Sensor Altitude: %f: ",
            klvData.value(LmCdl::VideoKlv601Key::SENSOR_TRUE_ALTITUDE, QVariant(MISSING_SENSOR_ALTITUDE)).toDouble());
    }
}

void VideoStreamMonitorContent::registerVideoStream(const QString& description, const QString& uri)
{
    ownedVideoStreams_.append(&videoStreamApiCollection_.registerStream(description, uri));
}

void VideoStreamMonitorContent::unregisterOwnedVideoStreams()
{
    foreach (LmCdl::I_VideoStreamApi* videoStreamApi, ownedVideoStreams_) {
        videoStreamApiCollection_.unregisterStream(*videoStreamApi);
    }
}
