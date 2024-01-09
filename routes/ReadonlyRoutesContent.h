#pragma once

/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <LmCdl/LoiterDirection.h>
#include <LmCdl/LoiterType.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QJsonObject>
#include <QObject>

class QTreeWidget;
class QTreeWidgetItem;

namespace LmCdl {
class I_VcsiVehicleExtensionApi;
class I_RouteApi;
class I_PlannedRouteCollection;
class I_DownloadedRouteCollection;
class StanagWaypoint;
}

class ReadonlyRoutesContent : public QObject {
    Q_OBJECT
public:
    ReadonlyRoutesContent(
        LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiVehicleExtensionApi& widgetApi, LmCdl::I_RouteApi& routes);
    virtual ~ReadonlyRoutesContent();

private slots:
    void populateTree();
    void addWaypointFields(QTreeWidgetItem* waypointItem, const LmCdl::StanagWaypoint& waypoint);
    void addLoiterFields(QTreeWidgetItem* waypointItem, const LmCdl::StanagWaypoint& waypoint);
    void addCustomProperties(QTreeWidgetItem* waypointItem, QJsonObject customPropertyExtension);

private:
    void populateTreeWithPlannedRouteCollection(
        QTreeWidgetItem* topLevelItem, LmCdl::I_PlannedRouteCollection& collection);
    void populateTreeWithDownloadedRouteCollection(
        QTreeWidgetItem* topLevelItem, LmCdl::I_DownloadedRouteCollection& collection);
    void resizeColumnsToFitContent();
    QString enumToString(const LmCdl::LoiterDirection loiterDirection);
    QString enumToString(const LmCdl::LoiterType loiterType);

    LmCdl::I_PlannedRouteCollection& plannedRoutes_;
    LmCdl::I_DownloadedRouteCollection& downloadedRoutes_;
    QTreeWidget* treeWidget_;
};
