/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/ActionResult.h>
#include <LmCdl/ExportDefines.h>
#include <LmCdl/Qt5QPromiseAdapter.h>
#include <QRunnable>

class DelayedActionPromiseFulfillment : public QRunnable {
public:
    explicit DelayedActionPromiseFulfillment(LmCdl::Qt5QPromiseAdapter<LmCdl::ActionResult> promise, int delayMs,
        LmCdl::ActionResult result = LmCdl::ActionResult::Success);
    virtual ~DelayedActionPromiseFulfillment();

    void run() override;

private:
    Q_DISABLE_COPY(DelayedActionPromiseFulfillment);

    LmCdl::Qt5QPromiseAdapter<LmCdl::ActionResult> promise_;
    int delayMs_;
    LmCdl::ActionResult result_;
};