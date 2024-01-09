/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <LmCdl/Checklist.h>
#include <LmCdl/ChecklistStep.h>
#include <QtGlobal>

class PreflightChecklistExampleChecklistBuilder {
public:
    explicit PreflightChecklistExampleChecklistBuilder(bool willPutManualStepFirst);
    virtual ~PreflightChecklistExampleChecklistBuilder();

    LmCdl::ChecklistStep manualStep;
    LmCdl::ChecklistStep automaticStep;
    LmCdl::Checklist output;
};
