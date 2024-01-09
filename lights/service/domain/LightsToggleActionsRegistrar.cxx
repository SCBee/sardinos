/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsToggleActionsRegistrar.h>

#include <I_LightsStateModel.h>
#include <LmCdl/Action.h>
#include <LmCdl/I_ActionRegistrationApi.h>
#include <LmCdl/Qt5QPromiseAdapter.h>
#include <QFuture>

LightsToggleActionsRegistrar::LightsToggleActionsRegistrar(I_LightsStateModel& stateModel,
    LmCdl::I_ActionRegistrationApi& actionRegistrationApi, LmCdl::UniqueIdentifier vehicleId)
    : stateModel_(stateModel)
    , actionRegistrationApi_(actionRegistrationApi)
    , vehicleId_(vehicleId)
{
    registerActionForLight("Nav", StanagLightType::Nav);
    registerActionForLight("NavIr", StanagLightType::NavIr);
    registerActionForLight("Strobe", StanagLightType::Strobe);
    registerActionForLight("StrobeIr", StanagLightType::StrobeIr);
    registerActionForLight("Nvd", StanagLightType::Nvd);
    registerActionForLight("Landing", StanagLightType::Landing);
    registerActionForLight("LandingIr", StanagLightType::LandingIr);
}

LightsToggleActionsRegistrar::~LightsToggleActionsRegistrar() { qDeleteAll(ownedActions_); }

void LightsToggleActionsRegistrar::registerActionForLight(const QString lightName, StanagLightType lightType)
{
    const QString SOURCE_NAME("LightsPlugin");
    const QString ACTION_NAME(lightName + QString(" light toggle for vehicle ") + QString::number(vehicleId_.value()));
    const QString DESCRIPTION(QString("Will toggle the state of the ") + lightName + QString(" light of vehicle ")
        + QString::number(vehicleId_.value()));

    std::function<QFuture<LmCdl::ActionResult>()> lightToggleAction([=]() {
        LmCdl::Qt5QPromiseAdapter<LmCdl::ActionResult> promise;
        QFuture<LmCdl::ActionResult> future = promise.future();

        I_SingleLightModel& model = stateModel_.lightModel(lightType);
        model.setCommandState(!model.isCommandedOn());

        promise.addResult(LmCdl::ActionResult::Success);
        promise.finish();
        return future;
    });

    ownedActions_.insert(
        actionRegistrationApi_.addAction(SOURCE_NAME, ACTION_NAME, DESCRIPTION, lightToggleAction).release());
}
