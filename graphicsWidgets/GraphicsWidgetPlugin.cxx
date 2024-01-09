/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <GraphicsWidgetPlugin.h>

#include <GraphicsWidgetContentCreator.h>
#include <LmCdl/I_GdtApi.h>
#include <LmCdl/I_QmlApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

GraphicsWidgetPlugin::GraphicsWidgetPlugin()
    : vehicleApi_(nullptr)
    , gdtApi_(nullptr)
    , applicationApi_(nullptr)
    , qmlApi_(nullptr)
{
}

GraphicsWidgetPlugin::~GraphicsWidgetPlugin() { }

QList<LmCdl::PluginRequirement> GraphicsWidgetPlugin::requiredApis() const
{
    return {
        LmCdl::PluginRequirement(VEHICLE_COLLECTION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(GDT_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0),
        LmCdl::PluginRequirement(QML_API_CAPABILITY_NAME, 1, 0, 0),
    };
}

LmCdl::PluginCapabilityIdentifier GraphicsWidgetPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool GraphicsWidgetPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == VEHICLE_COLLECTION_API_CAPABILITY_NAME) {
        vehicleApi_ = dynamic_cast<LmCdl::I_VehicleCollectionApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == GDT_API_CAPABILITY_NAME) {
        gdtApi_ = dynamic_cast<LmCdl::I_GdtApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == QML_API_CAPABILITY_NAME) {
        qmlApi_ = dynamic_cast<LmCdl::I_QmlApi*>(api);
        capabilityFound = true;
    }
    startPluginIfInitialized();
    return capabilityFound;
}

QObject* GraphicsWidgetPlugin::getProvidedApi() { return nullptr; }

bool GraphicsWidgetPlugin::isFullyInitialized() const { return (vehicleApi_ && gdtApi_ && applicationApi_ && qmlApi_); }

void GraphicsWidgetPlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        contentCreator_.reset(
            new GraphicsWidgetContentCreator(*vehicleApi_, *gdtApi_, applicationApi_->widgetExtensionApi().mapApi(),
                applicationApi_->widgetExtensionApi().vehicleApi(), *qmlApi_));
    }
}
