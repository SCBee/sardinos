/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>

namespace LmCdl {
class I_VideoStreamApiCollection;
class I_InteractableVideoDisplayApi;
class I_VideoVectorPickExclusiveListener;
class VideoVectorPick;
}

class VideoStreamVectorPickContent : public QObject {
    Q_OBJECT

public:
    explicit VideoStreamVectorPickContent(LmCdl::I_VideoStreamApiCollection& videoStreamApiCollection,
        LmCdl::I_InteractableVideoDisplayApi& interactableVideoDisplayApi);
    virtual ~VideoStreamVectorPickContent();

private slots:
    void printVectorPickInformation(const LmCdl::VideoVectorPick& videoVectorPick);
    void printActiveStatus(bool isActive);

private:
    void connectToApiSignals();

private:
    LmCdl::I_VideoStreamApiCollection& videoStreamApiCollection_;
    LmCdl::I_InteractableVideoDisplayApi& interactableVideoDisplayApi_;
    LmCdl::I_VideoVectorPickExclusiveListener& vectorPickListener_;
};
