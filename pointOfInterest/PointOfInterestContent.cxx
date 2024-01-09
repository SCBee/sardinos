/**
 \file
 Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
 */

#include <PointOfInterestContent.h>

#include <LmCdl/I_PointOfInterestApi.h>
#include <PointOfInterestContextMenuContent.h>
#include <PointOfInterestCrudContent.h>

PointOfInterestContent::PointOfInterestContent(LmCdl::I_PointOfInterestApi& pointOfInterestApi)
    : crudContent_(new PointOfInterestCrudContent(pointOfInterestApi))
    , mapContextMenuContent_(new PointOfInterestContextMenuContent(pointOfInterestApi))
{
}

PointOfInterestContent::~PointOfInterestContent() { }
