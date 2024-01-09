/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#include <ActionExamplePlugin.h>

#include <ActionExamplePluginContent.h>
#include <LmCdl/I_ActionApi.h>
#include <LmCdl/I_QmlApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QList>

ActionExamplePlugin::ActionExamplePlugin()
    : actionApi_(nullptr)
    , applicationApi_(nullptr)
    , qmlApi_(nullptr)
{
}

ActionExamplePlugin::~ActionExamplePlugin() { }

QList<LmCdl::PluginRequirement> ActionExamplePlugin::requiredApis() const
{
    QList<LmCdl::PluginRequirement> requirementList;
    requirementList.append(LmCdl::PluginRequirement(ACTION_CAPABILITY_NAME, 1, 0, 0));
    requirementList.append(LmCdl::PluginRequirement(VCSI_APPLICATION_API_CAPABILITY_NAME, 1, 0, 0));
    requirementList.append(LmCdl::PluginRequirement(QML_API_CAPABILITY_NAME, 1, 0, 0));
    return requirementList;
}

LmCdl::PluginCapabilityIdentifier ActionExamplePlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool ActionExamplePlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == ACTION_CAPABILITY_NAME) {
        actionApi_ = dynamic_cast<LmCdl::I_ActionApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == VCSI_APPLICATION_API_CAPABILITY_NAME) {
        applicationApi_ = dynamic_cast<LmCdl::I_VcsiApplicationApi*>(api);
        capabilityFound = true;
    }
    if (id.capabilityName() == QML_API_CAPABILITY_NAME) {
        qmlApi_ = dynamic_cast<LmCdl::I_QmlApi*>(api);
        if (qmlApi_) {
            capabilityFound = true;
        }
    }

    startPluginIfInitialized();
    return capabilityFound;
}

QObject* ActionExamplePlugin::getProvidedApi() { return nullptr; }

bool ActionExamplePlugin::isFullyInitialized() const { return (actionApi_ && applicationApi_ && qmlApi_); }

void ActionExamplePlugin::startPluginIfInitialized()
{
    if (isFullyInitialized()) {
        pluginContent_.reset(new ActionExamplePluginContent(*actionApi_, *applicationApi_, *qmlApi_));
    }
}
