/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <ExamplePayloadDrawingApiPluginContent.h>

#include <ExamplePayloadProjectedFootprintDrawing.h>
#include <LmCdl/I_PayloadDrawingApi.h>
#include <QList>

ExamplePayloadDrawingApiPluginContent::ExamplePayloadDrawingApiPluginContent(
    LmCdl::I_PayloadDrawingApi& payloadDrawingApi, LmCdl::I_GroundElevationApi& groundElevationApi)
    : payloadDrawingApi_(payloadDrawingApi)
    , domain_(groundElevationApi)
{
    foreach (auto projectedFootprintDrawing, domain_.projectedFootprintDrawings()) {
        payloadDrawingApi_.addDrawingForProjectedFootprint(*projectedFootprintDrawing);
    }
    foreach (auto starepointDrawing, domain_.starepointsDrawings()) {
        payloadDrawingApi_.addDrawingForStarepoint(*starepointDrawing);
    }
}

ExamplePayloadDrawingApiPluginContent::~ExamplePayloadDrawingApiPluginContent()
{
    foreach (auto starepointDrawing, domain_.starepointsDrawings()) {
        payloadDrawingApi_.removeDrawingForStarepoint(*starepointDrawing);
    }
    foreach (auto projectedFootprintDrawing, domain_.projectedFootprintDrawings()) {
        payloadDrawingApi_.removeDrawingForProjectedFootprint(*projectedFootprintDrawing);
    }
}
