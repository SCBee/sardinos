/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#include <PreflightChecklistExampleView.h>

#include <LmCdl/ChecklistStepModel.h>
#include <LmCdl/I_PreflightChecklistVisualizationApi.h>
#include <PreflightChecklistExampleBusinessLogic.h>
#include <PreflightChecklistExampleView.h>
#include <QQmlEngine>
#include <QUrl>

PreflightChecklistExampleView::PreflightChecklistExampleView(
    PreflightChecklistExampleBusinessLogic& businessLogic, LmCdl::I_PreflightChecklistVisualizationApi& api)
    : businessLogic_(businessLogic)
    , api_(api)
{
    qmlRegisterUncreatableType<LmCdl::ChecklistStepModel>(
        "LmCdl.Core", 1, 0, "ChecklistStepModel", "Type cannot be created in QML");

    api_.setUiForChecklistStep(businessLogic_.manualStep(), manualStepQmlSource(), businessLogic_);
    api_.setUiForChecklistStep(businessLogic_.automaticStep(), automaticStepQmlSource(), businessLogic_);
}

PreflightChecklistExampleView::~PreflightChecklistExampleView() { }

QUrl PreflightChecklistExampleView::manualStepQmlSource() const
{
    return qmlSource("PreflightChecklistExampleManualStep.qml");
}

QUrl PreflightChecklistExampleView::automaticStepQmlSource() const
{
    return qmlSource("PreflightChecklistExampleAutomaticStep.qml");
}

QUrl PreflightChecklistExampleView::qmlSource(const QString& fileName) const
{
    return QUrl(QString("modules/qml/PreflightChecklistExampleQuick/%1").arg(fileName));
}
