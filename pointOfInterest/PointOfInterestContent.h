/**
 \file
 Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <QScopedPointer>

class PointOfInterestContextMenuContent;
class PointOfInterestCrudContent;
namespace LmCdl {
class I_PointOfInterestApi;
}

class PointOfInterestContent {
public:
    explicit PointOfInterestContent(LmCdl::I_PointOfInterestApi& pointOfInterestApi);
    virtual ~PointOfInterestContent();

private:
    QScopedPointer<PointOfInterestCrudContent> crudContent_;
    QScopedPointer<PointOfInterestContextMenuContent> mapContextMenuContent_;
};
