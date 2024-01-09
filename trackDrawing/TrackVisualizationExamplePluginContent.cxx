/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <TrackVisualizationExamplePluginContent.h>

#include <ExampleTrackVisualization.h>
#include <LmCdl/I_TrackDrawingApi.h>
#include <QList>

TrackVisualizationExamplePluginContent::TrackVisualizationExamplePluginContent(
    LmCdl::I_TrackDrawingApi& trackDrawingApi)
    : trackDrawingApi_(trackDrawingApi)
    , domain_()
{
    foreach (ExampleTrackVisualization* track, domain_.tracks()) {
        trackDrawingApi_.addDrawingForTrack(*track);
    }
}

TrackVisualizationExamplePluginContent::~TrackVisualizationExamplePluginContent()
{
    foreach (ExampleTrackVisualization* track, domain_.tracks()) {
        trackDrawingApi_.removeDrawingForTrack(*track);
    }
}
