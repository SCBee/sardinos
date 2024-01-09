/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#include <VideoStreamContent.h>

#include <LmCdl/I_InteractableVideoDisplayApi.h>
#include <LmCdl/I_VideoStreamApiCollection.h>
#include <VideoStreamMonitorContent.h>
#include <VideoStreamVectorPickContent.h>

VideoStreamContent::VideoStreamContent(LmCdl::I_VideoStreamApiCollection& videoStreamApiCollection,
    LmCdl::I_InteractableVideoDisplayApi& interactableVideoDisplayApi)
    : vectorPickContent_(new VideoStreamVectorPickContent(videoStreamApiCollection, interactableVideoDisplayApi))
    , monitorContent_(new VideoStreamMonitorContent(videoStreamApiCollection))
{
}

VideoStreamContent::~VideoStreamContent() { }
