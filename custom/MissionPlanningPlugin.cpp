#include <MissionPlanningPlugin.h>

#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <MissionPlanningContentCreator.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <qimage.h>
#include <LmCdl/I_Billboard.h>
#include <qpalette.h>
#include <qcolor.h>
#include <qwidget.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_AreaSearchApi.h>

MissionPlanningPlugin::MissionPlanningPlugin()
    : pointOfInterestApi_(nullptr)
    , applicationApi_(nullptr)
    , vectorDrawingApi_(nullptr)
    , areaSearchApi_(nullptr)
{
}

MissionPlanningPlugin::~MissionPlanningPlugin() { }

QList<LmCdl::PluginRequirement> MissionPlanningPlugin::requiredApis() const
{
    return { 
                LmCdl::PluginRequirement(POINT_OF_INTEREST_API_CAPABILITY_NAME, 1, 0, 0),
                LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
                LmCdl::PluginRequirement(VECTOR_DATA_DRAWING_API_CAPABILITY_NAME, 1, 0, 0),
                LmCdl::PluginRequirement(AREA_SEARCH_API_CAPABILITY_NAME, 1, 0, 0),
            };
}

LmCdl::PluginCapabilityIdentifier MissionPlanningPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool MissionPlanningPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;

    if (id.capabilityName() == POINT_OF_INTEREST_API_CAPABILITY_NAME) {
        pointOfInterestApi_ = dynamic_cast<LmCdl::I_PointOfInterestApi*>(api);
        capabilityFound = true;
    }

    else if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }

    else if (id.capabilityName() == VECTOR_DATA_DRAWING_API_CAPABILITY_NAME) {
        vectorDrawingApi_ = dynamic_cast<LmCdl::I_VectorDataDrawingApi*>(api);
        capabilityFound = true;
    }

    else if (id.capabilityName() == AREA_SEARCH_API_CAPABILITY_NAME) {
        areaSearchApi_ = dynamic_cast<LmCdl::I_AreaSearchApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* MissionPlanningPlugin::getProvidedApi() { return nullptr; }

bool MissionPlanningPlugin::isFullyInitialized() const {  return (pointOfInterestApi_ && applicationApi_ && vectorDrawingApi_ && areaSearchApi_); }

void MissionPlanningPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContentCreator_.reset(new MissionPlanningContentCreator(applicationApi_->widgetExtensionApi().mapApi(), *pointOfInterestApi_, applicationApi_->userNotificationApi(), *vectorDrawingApi_, *areaSearchApi_));
    }
}
