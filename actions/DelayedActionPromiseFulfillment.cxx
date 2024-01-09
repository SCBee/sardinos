/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#include <DelayedActionPromiseFulfillment.h>
#include <QThread>

DelayedActionPromiseFulfillment::DelayedActionPromiseFulfillment(
    LmCdl::Qt5QPromiseAdapter<LmCdl::ActionResult> promise, int delayMs, LmCdl::ActionResult result)
    : promise_(std::move(promise))
    , delayMs_(delayMs)
    , result_(result)
{
}

DelayedActionPromiseFulfillment::~DelayedActionPromiseFulfillment() { }

void DelayedActionPromiseFulfillment::run()
{
    promise_.start();
    QThread::msleep(delayMs_);
    promise_.addResult(result_);
    promise_.finish();
}