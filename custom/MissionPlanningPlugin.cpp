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

MissionPlanningPlugin::MissionPlanningPlugin()
    : pointOfInterestApi_(nullptr)
    , applicationApi_(nullptr)
{
}

MissionPlanningPlugin::~MissionPlanningPlugin() { }

QList<LmCdl::PluginRequirement> MissionPlanningPlugin::requiredApis() const
{
    return { 
                LmCdl::PluginRequirement(POINT_OF_INTEREST_API_CAPABILITY_NAME, 1, 0, 0),
                LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
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
        if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* MissionPlanningPlugin::getProvidedApi() { return nullptr; }

bool MissionPlanningPlugin::isFullyInitialized() const {  return (pointOfInterestApi_ && applicationApi_); }

void MissionPlanningPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContentCreator_.reset(new MissionPlanningContentCreator(applicationApi_->widgetExtensionApi().mapApi(), *pointOfInterestApi_, applicationApi_->userNotificationApi()));
    }
}
