/**
 \file
 Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
 */

#include <PurgeSessionsExampleZeroizable.h>

#include <QtConcurrent>

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

PurgeSessionsExampleZeroizable::PurgeSessionsExampleZeroizable(QDir targetDirectory)
    : targetDirectory_(targetDirectory)
{
    connect(&operationWatcher_, &QFutureWatcher<void>::started, this, &PurgeSessionsExampleZeroizable::enabledChanged);
    connect(
        &operationWatcher_, &QFutureWatcher<void>::started, this, &PurgeSessionsExampleZeroizable::zeroizationStarted);

    connect(&operationWatcher_, &QFutureWatcher<void>::finished, this, &PurgeSessionsExampleZeroizable::enabledChanged);
    connect(&operationWatcher_, &QFutureWatcher<void>::finished, this,
        &PurgeSessionsExampleZeroizable::zeroizationComplete);
}

PurgeSessionsExampleZeroizable::~PurgeSessionsExampleZeroizable() = default;

QString PurgeSessionsExampleZeroizable::group() const { return "GCS"; }

QString PurgeSessionsExampleZeroizable::description() const { return "Per-sesssion logs / telemetry"; }

bool PurgeSessionsExampleZeroizable::enabled() const { return !operationWatcher_.isRunning(); }

void PurgeSessionsExampleZeroizable::zeroizeNow()
{
    if (enabled()) {
        QDir targetDirectory = targetDirectory_;
        auto operation = QtConcurrent::run([targetDirectory]() mutable {
            // this time delay is to demonstrate the visual zeroizing state
            std::this_thread::sleep_for(std::chrono::seconds(5));
            targetDirectory.removeRecursively();
        });
        operationWatcher_.setFuture(std::move(operation));
    }
}
