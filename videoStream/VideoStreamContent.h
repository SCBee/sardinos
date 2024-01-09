/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QScopedPointer>

class VideoStreamVectorPickContent;
class VideoStreamMonitorContent;

namespace LmCdl {
class I_VideoStreamApiCollection;
class I_InteractableVideoDisplayApi;
}

class VideoStreamContent {
public:
    explicit VideoStreamContent(LmCdl::I_VideoStreamApiCollection& videoStreamApiCollection,
        LmCdl::I_InteractableVideoDisplayApi& interactableVideoDisplayApi);
    virtual ~VideoStreamContent();

private:
    QScopedPointer<VideoStreamVectorPickContent> vectorPickContent_;
    QScopedPointer<VideoStreamMonitorContent> monitorContent_;
};
