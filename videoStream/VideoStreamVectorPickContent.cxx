/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#include <LmCdl/I_InteractableVideoDisplayApi.h>
#include <LmCdl/I_VideoStreamApiCollection.h>
#include <LmCdl/I_VideoVectorPickExclusiveListener.h>
#include <LmCdl/VideoVectorPick.h>
#include <LmCdl/VideoVectorPickActivityState.h>
#include <VideoStreamVectorPickContent.h>

VideoStreamVectorPickContent::VideoStreamVectorPickContent(LmCdl::I_VideoStreamApiCollection& videoStreamApiCollection,
    LmCdl::I_InteractableVideoDisplayApi& interactableVideoDisplayApi)
    : videoStreamApiCollection_(videoStreamApiCollection)
    , interactableVideoDisplayApi_(interactableVideoDisplayApi)
    , vectorPickListener_(interactableVideoDisplayApi_.registerVideoVectorPickListener())
{
    connectToApiSignals();
    interactableVideoDisplayApi_.setActiveVectorPickExclusiveListener(vectorPickListener_);
}

VideoStreamVectorPickContent::~VideoStreamVectorPickContent() { }

void VideoStreamVectorPickContent::connectToApiSignals()
{
    connect(&vectorPickListener_, &LmCdl::I_VideoVectorPickExclusiveListener::videoVectorPickChanged, this,
        &VideoStreamVectorPickContent::printVectorPickInformation);

    connect(&vectorPickListener_, SIGNAL(activeStatusChanged(bool)), this, SLOT(printActiveStatus(bool)));
}

void VideoStreamVectorPickContent::printVectorPickInformation(const LmCdl::VideoVectorPick& videoVectorPick)
{
    QString state = "Unknown";

    switch (videoVectorPick.activityState()) {
    case LmCdl::VideoVectorPickActivityState::Start: {
        state = "Start";
        break;
    }
    case LmCdl::VideoVectorPickActivityState::Update: {
        state = "Update";
        break;
    }
    case LmCdl::VideoVectorPickActivityState::Complete: {
        state = "Complete";
        break;
    }
    case LmCdl::VideoVectorPickActivityState::Cancel: {
        state = "Cancel";
        break;
    }
    }
    qDebug("\nState: %s\nStart: %d, %d\nEnd: %d, %d\nScreen Height: %d\nScreen Width: %d\n", qPrintable(state),
        videoVectorPick.startPoint().x(), videoVectorPick.startPoint().y(), videoVectorPick.endPoint().x(),
        videoVectorPick.endPoint().y(), videoVectorPick.displaySize().height(), videoVectorPick.displaySize().width());
}

void VideoStreamVectorPickContent::printActiveStatus(bool isActive)
{
    qDebug("Active Status: %s", isActive ? "True" : "False");
}