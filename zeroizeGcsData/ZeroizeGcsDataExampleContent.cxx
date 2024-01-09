/**
 \file
 Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
 */

#include <ZeroizeGcsDataExampleContent.h>

#include <LmCdl/I_VcsiStandardPaths.h>
#include <LmCdl/I_ZeroizableComponentApi.h>
#include <PurgeSessionsExampleZeroizable.h>

ZeroizeGcsDataExampleContent::ZeroizeGcsDataExampleContent(
    LmCdl::I_ZeroizableComponentApi& zeroizeApi, LmCdl::I_VcsiStandardPaths& pathsApi)
    : zeroizeApi_(zeroizeApi)
    , purgeSessionsZeroizable_(
          new PurgeSessionsExampleZeroizable(pathsApi.locateDirectory(LmCdl::I_VcsiStandardPaths::Location::Sessions)))
{
    registeredZeroizables_ << zeroizeApi_.addComponent(*purgeSessionsZeroizable_);
}

ZeroizeGcsDataExampleContent::~ZeroizeGcsDataExampleContent()
{
    foreach (const auto zeroizableId, registeredZeroizables_) {
        zeroizeApi_.removeComponent(zeroizableId);
    }
}
