/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#include <PreflightChecklistExampleChecklistBuilder.h>

#include <QUuid>

PreflightChecklistExampleChecklistBuilder::PreflightChecklistExampleChecklistBuilder(bool willPutManualStepFirst)
    : manualStep("manualStep", QUuid::createUuid())
    , automaticStep("automaticStep", QUuid::createUuid())
{
    if (willPutManualStepFirst) {
        output = LmCdl::Checklist(manualStep).andThen(automaticStep);
    } else {
        output = LmCdl::Checklist(automaticStep).andThen(manualStep);
    }
}

PreflightChecklistExampleChecklistBuilder::~PreflightChecklistExampleChecklistBuilder() { }
