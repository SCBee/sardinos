/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <ExamplePayloadDomain.h>

namespace LmCdl {
class I_GroundElevationApi;
class I_PayloadDrawingApi;
}

class ExamplePayloadDrawingApiPluginContent {
public:
    ExamplePayloadDrawingApiPluginContent(
        LmCdl::I_PayloadDrawingApi& payloadDrawingApi, LmCdl::I_GroundElevationApi& groundElevationApi);
    ~ExamplePayloadDrawingApiPluginContent();

private:
    LmCdl::I_PayloadDrawingApi& payloadDrawingApi_;
    ExamplePayloadDomain domain_;
};
