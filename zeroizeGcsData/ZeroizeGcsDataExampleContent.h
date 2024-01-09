/**
 \file
 Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QScopedPointer>
#include <QVector>
#include <QtGlobal>

class PurgeSessionsExampleZeroizable;

namespace LmCdl {
class I_VcsiStandardPaths;
class I_ZeroizableComponentApi;
}

class ZeroizeGcsDataExampleContent {
public:
    ZeroizeGcsDataExampleContent(LmCdl::I_ZeroizableComponentApi& zeroizeApi, LmCdl::I_VcsiStandardPaths& pathsApi);
    ~ZeroizeGcsDataExampleContent();

private:
    Q_DISABLE_COPY(ZeroizeGcsDataExampleContent)

    LmCdl::I_ZeroizableComponentApi& zeroizeApi_;

    QVector<LmCdl::UniqueIdentifier> registeredZeroizables_;

    QScopedPointer<PurgeSessionsExampleZeroizable> purgeSessionsZeroizable_;
};
