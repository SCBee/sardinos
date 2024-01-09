/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <ExampleTrackDomain.h>

namespace LmCdl {
class I_TrackDrawingApi;
}

class TrackVisualizationExamplePluginContent {
public:
    TrackVisualizationExamplePluginContent(LmCdl::I_TrackDrawingApi& trackDrawingApi);
    ~TrackVisualizationExamplePluginContent();

private:
    LmCdl::I_TrackDrawingApi& trackDrawingApi_;
    ExampleTrackDomain domain_;
};