#include <QList>
#include <iostream>

#include <LmCdl/I_Billboard.h>
#include <LmCdl/I_MissionDrawingApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_RouteApi.h>
#include <LmCdl/I_TrackDrawingApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_VideoStreamApiCollection.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <MissionPlanningContentCreator.h>
#include <MissionPlanningPlugin.h>

MissionPlanningPlugin::MissionPlanningPlugin()
    : pointOfInterestApi_(nullptr)
    , applicationApi_(nullptr)
    , vectorDrawingApi_(nullptr)
    , missionDrawingApi_(nullptr)
    , routeApi_(nullptr)
    , trackApi_(nullptr)
    , videoCollectionApi_(nullptr)
{
}

MissionPlanningPlugin::~MissionPlanningPlugin() = default;

QList<LmCdl::PluginRequirement> MissionPlanningPlugin::requiredApis() const
{
    return {
        LmCdl::PluginRequirement(
            POINT_OF_INTEREST_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(
            VECTOR_DATA_DRAWING_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(MISSION_DRAWING_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(ROUTE_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(TRACK_DRAWING_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(
            VIDEO_STREAM_API_COLLECTION_CAPABILITY_NAME, 1, 0, 0)};
}

LmCdl::PluginCapabilityIdentifier MissionPlanningPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool MissionPlanningPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id,
                                           QObject* api)
{
    bool capabilityFound = false;

    if (id.capabilityName() == POINT_OF_INTEREST_API_CAPABILITY_NAME) {
        pointOfInterestApi_ = dynamic_cast<LmCdl::I_PointOfInterestApi*>(api);
        capabilityFound     = true;
    }

    else if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME)
    {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }

    else if (id.capabilityName() == VECTOR_DATA_DRAWING_API_CAPABILITY_NAME)
    {
        vectorDrawingApi_ = dynamic_cast<LmCdl::I_VectorDataDrawingApi*>(api);
        capabilityFound   = true;
    }

    else if (id.capabilityName() == MISSION_DRAWING_API_CAPABILITY_NAME)
    {
        missionDrawingApi_ = dynamic_cast<LmCdl::I_MissionDrawingApi*>(api);
        capabilityFound    = true;
    }

    else if (id.capabilityName() == ROUTE_API_CAPABILITY_NAME)
    {
        routeApi_       = dynamic_cast<LmCdl::I_RouteApi*>(api);
        capabilityFound = true;
    }

    else if (id.capabilityName() == TRACK_DRAWING_API_CAPABILITY_NAME)
    {
        trackApi_       = dynamic_cast<LmCdl::I_TrackDrawingApi*>(api);
        capabilityFound = true;
    }

    else if (id.capabilityName() == VIDEO_STREAM_API_COLLECTION_CAPABILITY_NAME)
    {
        videoCollectionApi_ =
            dynamic_cast<LmCdl::I_VideoStreamApiCollection*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* MissionPlanningPlugin::getProvidedApi()
{
    return nullptr;
}

bool MissionPlanningPlugin::isFullyInitialized() const
{
    return (pointOfInterestApi_ && applicationApi_ && vectorDrawingApi_
            && missionDrawingApi_ && routeApi_ && trackApi_
            && videoCollectionApi_);
}

void MissionPlanningPlugin::startPluginIfInitialized()
{
    // Some preproc stuff for debugging using a console window :)
    // we can delete this when we finish everything
    // -- zach
#ifdef DEBUG
    FreeConsole();

    AllocConsole();

    AttachConsole(GetCurrentProcessId());

    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    freopen("CON", "r", stdin);
#endif

    if (isFullyInitialized()) {
        pluginContentCreator_.reset(new MissionPlanningContentCreator(
            applicationApi_->widgetExtensionApi().mapApi(),
            *pointOfInterestApi_,
            applicationApi_->userNotificationApi(),
            *vectorDrawingApi_,
            *missionDrawingApi_,
            *routeApi_,
            *trackApi_,
            *videoCollectionApi_));
    }
}
