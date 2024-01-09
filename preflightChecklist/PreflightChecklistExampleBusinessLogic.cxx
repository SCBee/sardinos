/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#include <PreflightChecklistExampleBusinessLogic.h>

#include <LmCdl/ChecklistStepModel.h>
#include <LmCdl/I_VehiclePreflightChecklist.h>
#include <PreflightChecklistExampleChecklistBuilder.h>
#include <QStringList>
#include <algorithm>

namespace {
QString describe(LmCdl::ChecklistStepStatus status)
{
    QString result("Invalid");

    switch (status) {
    case LmCdl::ChecklistStepStatus::Passed:
        result = QString("Passed");
        break;
    case LmCdl::ChecklistStepStatus::Skipped:
        result = QString("Skipped");
        break;
    case LmCdl::ChecklistStepStatus::Failed:
        result = QString("Failed");
    }

    return result;
}
}

PreflightChecklistExampleBusinessLogic::PreflightChecklistExampleBusinessLogic(
    LmCdl::I_VehiclePreflightChecklist& vehiclePreflightChecklist, bool willPutManualStepFirst)
    : vehiclePreflightChecklist_(vehiclePreflightChecklist)
    , builder_(new PreflightChecklistExampleChecklistBuilder(willPutManualStepFirst))
    , manualStepModel_(
          new LmCdl::ChecklistStepModel(builder_->manualStep, vehiclePreflightChecklist_.checklistRunner()))
    , automaticStepModel_(
          new LmCdl::ChecklistStepModel(builder_->automaticStep, vehiclePreflightChecklist_.checklistRunner()))
{
    vehiclePreflightChecklist_.updateChecklist(builder_->output);

    connect(&vehiclePreflightChecklist_.checklistRunner(), &LmCdl::ChecklistRunner::started, this,
        &PreflightChecklistExampleBusinessLogic::resultsChanged);
    connect(&vehiclePreflightChecklist_.checklistRunner(), &LmCdl::ChecklistRunner::finished, this,
        &PreflightChecklistExampleBusinessLogic::resultsChanged);
    connect(automaticStepModel_.data(), &LmCdl::ChecklistStepModel::stepStarted, this,
        &PreflightChecklistExampleBusinessLogic::setAutomatedStepResultToOppositeOfManualStepResult);
}

PreflightChecklistExampleBusinessLogic::~PreflightChecklistExampleBusinessLogic() { }

Q_INVOKABLE void PreflightChecklistExampleBusinessLogic::run() { vehiclePreflightChecklist_.runPreflightChecklist(); }

LmCdl::ChecklistStep& PreflightChecklistExampleBusinessLogic::manualStep() { return builder_->manualStep; }

LmCdl::ChecklistStep& PreflightChecklistExampleBusinessLogic::automaticStep() { return builder_->automaticStep; }

LmCdl::ChecklistStepModel* PreflightChecklistExampleBusinessLogic::manualStepModel() { return manualStepModel_.data(); }

LmCdl::ChecklistStepModel* PreflightChecklistExampleBusinessLogic::automaticStepModel()
{
    return automaticStepModel_.data();
}

QString PreflightChecklistExampleBusinessLogic::results() const
{
    LmCdl::ChecklistResult results = vehiclePreflightChecklist_.checklistRunner().checklistResult();
    QStringList resultList;
    auto toString = [](const std::pair<LmCdl::ChecklistStep, LmCdl::ChecklistStepStatus>& kv) {
        return QString("%1: %2").arg(kv.first.description(), describe(kv.second));
    };
    std::transform(results.status.constKeyValueBegin(), results.status.constKeyValueEnd(),
        std::back_inserter(resultList), toString);
    return resultList.join("\n");
}

void PreflightChecklistExampleBusinessLogic::setAutomatedStepResultToOppositeOfManualStepResult()
{
    // In a production example, this would delegate to the relevant business logic before deciding
    // whether an automated step passed or failed, here we just do the opposite of whatever the
    // user selected in the manual step.
    if (manualStepModel_->passed()) {
        automaticStepModel_->fail();
    } else {
        automaticStepModel_->pass();
    }
    automaticStepModel_->nextStep();
}
