/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <QObject>

class PreflightChecklistExampleChecklistBuilder;

namespace LmCdl {
class ChecklistRunner;
class ChecklistStep;
class ChecklistStepModel;
class I_VehiclePreflightChecklist;
}

class PreflightChecklistExampleBusinessLogic : public QObject {
    Q_OBJECT

    Q_PROPERTY(LmCdl::ChecklistStepModel* manualStepModel READ manualStepModel CONSTANT)
    Q_PROPERTY(LmCdl::ChecklistStepModel* automaticStepModel READ automaticStepModel CONSTANT)
    Q_PROPERTY(QString results READ results NOTIFY resultsChanged)

public:
    explicit PreflightChecklistExampleBusinessLogic(
        LmCdl::I_VehiclePreflightChecklist& vehiclePreflightChecklist, bool willPutManualStepFirst);
    ~PreflightChecklistExampleBusinessLogic() override;

    Q_INVOKABLE void run();

    LmCdl::ChecklistStep& manualStep();
    LmCdl::ChecklistStep& automaticStep();
    LmCdl::ChecklistStepModel* manualStepModel();
    LmCdl::ChecklistStepModel* automaticStepModel();
    QString results() const;

signals:
    void resultsChanged();

private slots:
    void setAutomatedStepResultToOppositeOfManualStepResult();

private:
    Q_DISABLE_COPY(PreflightChecklistExampleBusinessLogic);

    LmCdl::I_VehiclePreflightChecklist& vehiclePreflightChecklist_;
    QScopedPointer<PreflightChecklistExampleChecklistBuilder> builder_;
    QScopedPointer<LmCdl::ChecklistStepModel> manualStepModel_;
    QScopedPointer<LmCdl::ChecklistStepModel> automaticStepModel_;
};
