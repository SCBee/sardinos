/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QList>
#include <QObject>
#include <QTimer>

namespace LmCdl {
class I_VideoStreamApiCollection;
class I_VideoStreamApi;
}

class VideoStreamMonitorContent : public QObject {
    Q_OBJECT

public:
    explicit VideoStreamMonitorContent(LmCdl::I_VideoStreamApiCollection& videoStreamApiCollection);
    virtual ~VideoStreamMonitorContent();

private slots:
    void addToKnownVideoStreams(LmCdl::I_VideoStreamApi& videoStreamApi);
    void removeFromKnownVideoStreams(LmCdl::I_VideoStreamApi& videoStreamApi);
    void logSnapshotInfo();

private:
    void initializeSnapshotTimer();
    void connectToApiSignals();
    void registerVideoStream(const QString& description, const QString& uri);
    void unregisterOwnedVideoStreams();

private:
    LmCdl::I_VideoStreamApiCollection& videoStreamApiCollection_;
    QList<LmCdl::I_VideoStreamApi*> ownedVideoStreams_;
    QList<LmCdl::I_VideoStreamApi*> allKnownVideoStreams_;
    QTimer snapshotTimer_;
};
