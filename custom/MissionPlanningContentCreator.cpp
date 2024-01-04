/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <MissionPlanningContentCreator.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <qwidget.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <qicon.h>
#include <LmCdl/I_Billboard.h>
#include <LmCdl/ContextMenuEvent.h>
#include <QLabel>
#include <QTimer>
#include <string>
#include <sstream>
#include <iomanip>

MissionPlanningContentCreator::MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_PointOfInterestApi& poiApi, LmCdl::I_VcsiUserNotificationApi& notApi)
    : contextMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
    , poiApi_(poiApi)
    , notApi_(notApi)
{
    contextMenuItem_.setBackgroundColor(*new QColor(235, 12, 12, 180));
    contextMenuItem_.setDescription("Add Mission Bound");
    contextMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    contextMenuItem_.setIcon(":/MissionPlanning/missionPlanningDinoIcon");
    contextMenuItem_.setVisible(true);
    connectToApiSignals();
}

MissionPlanningContentCreator::~MissionPlanningContentCreator() { }

void MissionPlanningContentCreator::connectToApiSignals()
{
    connect(&contextMenuItem_,
        &LmCdl::I_ContextMenuItem::clicked, this,
        &MissionPlanningContentCreator::getPoiProperties);
}

void MissionPlanningContentCreator::getPoiProperties(const LmCdl::ContextMenuEvent &event) 
{
    auto location = event.worldLocation();

    auto properties = new LmCdl::VcsiPointOfInterestProperties(*new QString("Mission Bound"), location);

    auto id = new LmCdl::VcsiPointOfInterestId();

    publishAndMapPointOfInterest(*id, *properties);

    auto label = new QLabel();

    std::stringstream ss;
    
    ss << "Mission bound placed at " << std::fixed << std::setprecision(5) << location.latitude() << ", " << std::fixed << std::setprecision(5) << location.longitude(); 

    std::string xString = ss.str();

    label->setText(xString.c_str());

    auto removeTimer = new QTimer();

    removeTimer->setInterval(3000);

    connect(removeTimer, &QTimer::timeout, this, &MissionPlanningContentCreator::removeNotification);

    removeTimer->start();

    notification_ = &notApi_.addNotification(label);
}

void MissionPlanningContentCreator::removeNotification()
{
    notApi_.removeNotification(*notification_);
    notification_ = NULL;
}

void MissionPlanningContentCreator::publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId sourceId, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest)
{
    auto mapIds = [this, sourceId](const LmCdl::VcsiPointOfInterestId& cloneId) { };

    poiApi_.addPointOfInterest(pointOfInterest, mapIds);
}