/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <QObject>
#include <QPointer>

class PreflightChecklistExampleBusinessLogic;

namespace LmCdl {
class I_PreflightChecklistVisualizationApi;
}

class PreflightChecklistExampleView : public QObject {
    Q_OBJECT
public:
    explicit PreflightChecklistExampleView(
        PreflightChecklistExampleBusinessLogic& businessLogic, LmCdl::I_PreflightChecklistVisualizationApi& api);
    ~PreflightChecklistExampleView() override;

private:
    Q_DISABLE_COPY(PreflightChecklistExampleView);

    QUrl manualStepQmlSource() const;
    QUrl automaticStepQmlSource() const;
    QUrl qmlSource(const QString& fileName) const;

private:
    PreflightChecklistExampleBusinessLogic& businessLogic_;
    LmCdl::I_PreflightChecklistVisualizationApi& api_;
};
