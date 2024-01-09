//
// Created by dev on 1/9/2024.
//
#include "TestContentCreator.h"

#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_Billboard.h>
#include <LmCdl/ContextMenuEvent.h>

#include <QLabel>
#include <QTimer>
#include <qwidget.h>
#include <qicon.h>

TestContentCreator::TestContentCreator(LmCdl::I_VcsiMapExtensionApi &mapApi,
                                       LmCdl::I_PointOfInterestApi &poiApi,
                                       LmCdl::I_VcsiUserNotificationApi &notApi)
        : contextMenuItem_(mapApi.terrainContextMenu().registerMenuItem()), poiApi_(poiApi), notApi_(notApi),
          notification_(nullptr) {
    contextMenuItem_.setBackgroundColor(*std::make_unique<QColor>(235, 12, 12, 180));
    contextMenuItem_.setDescription("Add Mission Bound");
    contextMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    contextMenuItem_.setIcon(":/MissionPlanning/missionPlanningDinoIcon");
    contextMenuItem_.setVisible(true);
    connectToApiSignals();
}

TestContentCreator::~TestContentCreator() = default;

void TestContentCreator::connectToApiSignals() {
    connect(&contextMenuItem_,
            &LmCdl::I_ContextMenuItem::clicked, this,
            &TestContentCreator::getPoiProperties);
}

void TestContentCreator::getPoiProperties(const LmCdl::ContextMenuEvent &event) {
    const auto &location = event.worldLocation();

    auto properties = std::make_unique<LmCdl::VcsiPointOfInterestProperties>("Mission Bound", location);
    auto id = std::make_unique<LmCdl::VcsiPointOfInterestId>();

    publishAndMapPointOfInterest(*id, *properties);

    auto label = std::make_unique<QLabel>();

    label->setText(
            QString("Mission bound placed at %1, %2").arg(location.latitude(), 0, 'f', 5).arg(location.longitude(), 0,
                                                                                              'f', 5));

    auto removeTimer = std::make_unique<QTimer>();
    removeTimer->setInterval(3000);
    connect(removeTimer.get(), &QTimer::timeout, this, &TestContentCreator::removeNotification);
    removeTimer->start();

    notification_ = &notApi_.addNotification(label.release());
}

void TestContentCreator::removeNotification() {
    notApi_.removeNotification(*notification_);
    notification_ = nullptr;
}

void TestContentCreator::publishAndMapPointOfInterest(const LmCdl::VcsiPointOfInterestId &sourceId,
                                                      const LmCdl::VcsiPointOfInterestProperties &pointOfInterest) {
    auto mapIds = [sourceId](const LmCdl::VcsiPointOfInterestId &cloneId) {};

    poiApi_.addPointOfInterest(pointOfInterest, mapIds);
}
