//
// Created by dev on 1/8/2024.
//

#include "TestPlugin.h"
#include "TestContentCreator.h"

#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_Billboard.h>

#include <qimage.h>
#include <qpalette.h>
#include <qwidget.h>

TestPlugin::TestPlugin() : pointOfInterestApi_(nullptr), applicationApi_(nullptr) {}

TestPlugin::~TestPlugin() = default;

QList<LmCdl::PluginRequirement> TestPlugin::requiredApis() const {
    return {
            LmCdl::PluginRequirement(POINT_OF_INTEREST_API_CAPABILITY_NAME, 1, 0, 0),
            LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0)
    };
}

LmCdl::PluginCapabilityIdentifier TestPlugin::providedApi() const {
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool TestPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject *api) {
    bool capabilityFound = false;

    if (id.capabilityName() == POINT_OF_INTEREST_API_CAPABILITY_NAME) {
        pointOfInterestApi_ = dynamic_cast<LmCdl::I_PointOfInterestApi *>(api);
    } else if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi *>(api);
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject *TestPlugin::getProvidedApi() { return nullptr; }

bool TestPlugin::isFullyInitialized() const { return (pointOfInterestApi_ && applicationApi_); }

void TestPlugin::startPluginIfInitialized() {
    if (isFullyInitialized()) {
        pluginContentCreator_ = std::make_unique<TestContentCreator>(
                applicationApi_->widgetExtensionApi().mapApi(),
                *pointOfInterestApi_,
                applicationApi_->userNotificationApi()
        );
    }
}
