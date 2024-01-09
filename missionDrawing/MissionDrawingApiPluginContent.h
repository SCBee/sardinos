/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <ExampleMissionDomain.h>

namespace LmCdl {
class I_MissionDrawingApi;
}

class MissionDrawingApiPluginContent {
public:
    MissionDrawingApiPluginContent(LmCdl::I_MissionDrawingApi& missionDrawingApi);
    ~MissionDrawingApiPluginContent();

private:
    LmCdl::I_MissionDrawingApi& missionDrawingApi_;
    ExampleMissionDomain domain_;
};