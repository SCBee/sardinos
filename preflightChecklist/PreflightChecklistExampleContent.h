/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QScopedPointer>
#include <QtGlobal>

class PreflightChecklistExampleBusinessLogic;
class PreflightChecklistExampleView;

namespace LmCdl {
class I_PreflightChecklistApi;
class I_PreflightChecklistVisualizationApi;
}

class PreflightChecklistExampleContent : public QObject {
    Q_OBJECT
public:
    explicit PreflightChecklistExampleContent(LmCdl::I_PreflightChecklistApi& preflightChecklistApi,
        LmCdl::I_PreflightChecklistVisualizationApi& preflightChecklistVisualizationApi);
    virtual ~PreflightChecklistExampleContent();

private slots:
    void instantiateVehicleChecklistBusinessLogicAndView(LmCdl::UniqueIdentifier vehicleIdentification);

private:
    Q_DISABLE_COPY(PreflightChecklistExampleContent);

    LmCdl::I_PreflightChecklistApi& preflightChecklistApi_;
    LmCdl::I_PreflightChecklistVisualizationApi& preflightChecklistVisualizationApi_;
    QScopedPointer<PreflightChecklistExampleBusinessLogic> businessLogic1_;
    QScopedPointer<PreflightChecklistExampleView> view1_;
    QScopedPointer<PreflightChecklistExampleBusinessLogic> businessLogic2_;
    QScopedPointer<PreflightChecklistExampleView> view2_;
};
