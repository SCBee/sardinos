/**
 \file
 Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/I_Zeroizable.h>
#include <QDir>
#include <QFutureWatcher>
//
class PurgeSessionsExampleZeroizable : public LmCdl::I_Zeroizable {
    Q_OBJECT

public:
    explicit PurgeSessionsExampleZeroizable(QDir targetDirectory);
    ~PurgeSessionsExampleZeroizable();

    QString group() const override;
    QString description() const override;

    bool enabled() const override;
    void zeroizeNow() override;

private:
    Q_DISABLE_COPY(PurgeSessionsExampleZeroizable)

    QDir targetDirectory_;
    QFutureWatcher<void> operationWatcher_;
};
