/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <MissionPlanningContentCreator.h>
#include <MissionPlanningContent.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <qwidget.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <qicon.h>
#include <LmCdl/I_Billboard.h>

MissionPlanningContentCreator::MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_PointOfInterestApi& poiApi)
    : content_(new MissionPlanningContent(mapApi, poiApi))
{
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() { }

