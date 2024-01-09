/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <ReadonlyRoutesContent.h>

#include <LmCdl/I_DownloadedRouteCollection.h>
#include <LmCdl/I_PlannedRouteCollection.h>
#include <LmCdl/I_RouteApi.h>
#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/StanagRoute.h>
#include <LmCdl/StanagWaypoint.h>
#include <LmCdl/TabWidgetMetadata.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <qmath.h>

namespace {
const int KEY_COLUMN = 0;
const int VALUE_COLUMN = 1;

double calculateDistance(const LmCdl::StanagWaypoint& waypoint, LmCdl::DistanceUnit unit)
{
    const double loiterDiameter = waypoint.loiterRadius.value(unit) * 2;
    const double loiterLength = waypoint.loiterLength.value(unit);
    const double loiterDiagonal = qSqrt(loiterDiameter * loiterDiameter + loiterLength * loiterLength);
    double totalDistance = 0.;
    switch (waypoint.loiterType) {
    case LmCdl::LoiterType::Circular:
        totalDistance = loiterDiameter * M_PI;
        break;
    case LmCdl::LoiterType::Racetrack:
        totalDistance = loiterDiameter * M_PI + loiterLength * 2;
        break;
    case LmCdl::LoiterType::Figure8:
        totalDistance = loiterDiameter * M_PI + loiterDiagonal * 2;
        break;
    case LmCdl::LoiterType::Hover:
    case LmCdl::LoiterType::Invalid:
    default:
        return 0.;
    }
    return totalDistance;
}

double calculateLapTime(const LmCdl::StanagWaypoint& waypoint)
{
    double totalDistance = calculateDistance(waypoint, LmCdl::DistanceUnit::Meters);
    return totalDistance / waypoint.speed.value(LmCdl::SpeedUnit::MetersPerSecond);
}
}

ReadonlyRoutesContent::ReadonlyRoutesContent(
    LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VcsiVehicleExtensionApi& widgetApi, LmCdl::I_RouteApi& routes)
    : plannedRoutes_(routes.plannedRoutes())
    , downloadedRoutes_(routes.downloadedRoutes(vehicleId))
    , treeWidget_(new QTreeWidget())
{
    treeWidget_->setColumnCount(2);
    treeWidget_->setColumnWidth(0, 120);
    treeWidget_->setIndentation(10);
    treeWidget_->setHeaderLabels(QStringList() << "Routes and Waypoints"
                                               << "Properties");
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    QPushButton* refreshButton = new QPushButton;
    refreshButton->setText("Refresh");
    layout->addWidget(treeWidget_);
    layout->addWidget(refreshButton);
    widget->setLayout(layout);
    widgetApi.addSettingsMenu(vehicleId, LmCdl::TabWidgetMetadata("Routes and waypoints"), widget);

    connect(refreshButton, SIGNAL(clicked()), this, SLOT(populateTree()));
}

ReadonlyRoutesContent::~ReadonlyRoutesContent() { }

void ReadonlyRoutesContent::populateTree()
{
    treeWidget_->clear();

    QTreeWidgetItem* downloadedRoutesItem = new QTreeWidgetItem(treeWidget_);
    downloadedRoutesItem->setText(KEY_COLUMN, "Downloaded Routes");

    QTreeWidgetItem* plannedRoutesItem = new QTreeWidgetItem(treeWidget_);
    plannedRoutesItem->setText(KEY_COLUMN, "Planned Routes");

    treeWidget_->addTopLevelItems({ downloadedRoutesItem, plannedRoutesItem });

    populateTreeWithDownloadedRouteCollection(downloadedRoutesItem, downloadedRoutes_);
    populateTreeWithPlannedRouteCollection(plannedRoutesItem, plannedRoutes_);

    resizeColumnsToFitContent();
}

void ReadonlyRoutesContent::populateTreeWithPlannedRouteCollection(
    QTreeWidgetItem* topLevelItem, LmCdl::I_PlannedRouteCollection& collection)
{
    foreach (LmCdl::UniqueIdentifier routeId, collection.routes()) {
        LmCdl::StanagRoute route = collection.lookupRoute(routeId);
        QTreeWidgetItem* newTopLevelItem = new QTreeWidgetItem(topLevelItem);
        newTopLevelItem->setFirstColumnSpanned(true);
        newTopLevelItem->setText(KEY_COLUMN, route.routeName);

        foreach (LmCdl::UniqueIdentifier waypointId, route.waypoints) {
            QTreeWidgetItem* newWaypointItem = new QTreeWidgetItem(newTopLevelItem);
            newWaypointItem->setFirstColumnSpanned(true);
            newWaypointItem->setText(KEY_COLUMN, QString("Waypoint %1").arg(waypointId.value()));
            addWaypointFields(newWaypointItem, collection.lookupWaypoint(waypointId));
        }
    }
}

void ReadonlyRoutesContent::populateTreeWithDownloadedRouteCollection(
    QTreeWidgetItem* topLevelItem, LmCdl::I_DownloadedRouteCollection& collection)
{
    foreach (LmCdl::UniqueIdentifier routeId, collection.routes()) {
        LmCdl::StanagRoute route = collection.lookupRoute(routeId);
        QTreeWidgetItem* newTopLevelItem = new QTreeWidgetItem(topLevelItem);
        newTopLevelItem->setFirstColumnSpanned(true);
        newTopLevelItem->setText(KEY_COLUMN, route.routeName);

        foreach (LmCdl::UniqueIdentifier waypointId, route.waypoints) {
            QTreeWidgetItem* newWaypointItem = new QTreeWidgetItem(newTopLevelItem);
            newWaypointItem->setFirstColumnSpanned(true);
            newWaypointItem->setText(KEY_COLUMN, QString("Waypoint %1").arg(waypointId.value()));
            addWaypointFields(newWaypointItem, collection.lookupWaypoint(waypointId));
        }
    }
}

void ReadonlyRoutesContent::resizeColumnsToFitContent()
{
    treeWidget_->resizeColumnToContents(0);
    treeWidget_->resizeColumnToContents(1);
}

void ReadonlyRoutesContent::addWaypointFields(QTreeWidgetItem* waypointItem, const LmCdl::StanagWaypoint& waypoint)
{
    QTreeWidgetItem* newLatitudeItem = new QTreeWidgetItem(waypointItem);
    newLatitudeItem->setText(KEY_COLUMN, "latitude");
    newLatitudeItem->setText(VALUE_COLUMN, QString("%1 degrees").arg(waypoint.location.latitude()));

    QTreeWidgetItem* newLongitudeItem = new QTreeWidgetItem(waypointItem);
    newLongitudeItem->setText(KEY_COLUMN, "longitude");
    newLongitudeItem->setText(VALUE_COLUMN, QString("%1 degrees").arg(waypoint.location.longitude()));

    QTreeWidgetItem* newAltitudeItem = new QTreeWidgetItem(waypointItem);
    newAltitudeItem->setText(KEY_COLUMN, "altitude");
    newAltitudeItem->setText(VALUE_COLUMN, QString("%1 meters").arg(waypoint.location.altitude()));

    QTreeWidgetItem* newSpeedItem = new QTreeWidgetItem(waypointItem);
    newSpeedItem->setText(KEY_COLUMN, "speed");
    newSpeedItem->setText(VALUE_COLUMN, QString("%1 kts").arg(waypoint.speed.value(LmCdl::SpeedUnit::Knots)));

    QTreeWidgetItem* newNextWaypointItem = new QTreeWidgetItem(waypointItem);
    newNextWaypointItem->setText(KEY_COLUMN, "nextWaypointId");
    newNextWaypointItem->setText(VALUE_COLUMN, QString::number(waypoint.nextWaypointId.value()));

    addLoiterFields(waypointItem, waypoint);

    addCustomProperties(waypointItem, waypoint.customPropertyExtension);
}

void ReadonlyRoutesContent::addLoiterFields(QTreeWidgetItem* waypointItem, const LmCdl::StanagWaypoint& waypoint)
{
    if (waypoint.loiterTime.toSeconds() <= 0) {
        return;
    }
    QTreeWidgetItem* newLoiterItem = new QTreeWidgetItem(waypointItem);
    newLoiterItem->setFirstColumnSpanned(true);
    newLoiterItem->setText(KEY_COLUMN, QString("Loiter"));

    QTreeWidgetItem* newLoiterTypeItem = new QTreeWidgetItem(newLoiterItem);
    newLoiterTypeItem->setText(KEY_COLUMN, QString("type"));
    newLoiterTypeItem->setText(VALUE_COLUMN, enumToString(waypoint.loiterType));

    QTreeWidgetItem* newLoiterTimeItem = new QTreeWidgetItem(newLoiterItem);
    newLoiterTimeItem->setText(KEY_COLUMN, QString("time"));
    newLoiterTimeItem->setText(VALUE_COLUMN, QString::number(waypoint.loiterTime.toSeconds()));

    QTreeWidgetItem* newLoiterBearingItem = new QTreeWidgetItem(newLoiterItem);
    newLoiterBearingItem->setText(KEY_COLUMN, QString("bearing"));
    newLoiterBearingItem->setText(
        1, QString("%1%2T").arg(waypoint.loiterBearing.value(LmCdl::AngleUnit::Degrees), 0, 'f', 0).arg(QChar(0260)));

    QTreeWidgetItem* newLoiterDirectionItem = new QTreeWidgetItem(newLoiterItem);
    newLoiterDirectionItem->setText(KEY_COLUMN, QString("direction"));
    newLoiterDirectionItem->setText(VALUE_COLUMN, enumToString(waypoint.loiterDirection));

    QTreeWidgetItem* newLoiterDistanceItem = new QTreeWidgetItem(newLoiterItem);
    newLoiterDistanceItem->setText(KEY_COLUMN, QString("lap distance"));
    newLoiterDistanceItem->setText(
        1, QString("%1 m").arg(calculateDistance(waypoint, LmCdl::DistanceUnit::Meters), 0, 'f', 0));

    QTreeWidgetItem* newLoiterLapTimeItem = new QTreeWidgetItem(newLoiterItem);
    newLoiterLapTimeItem->setText(KEY_COLUMN, QString("lap time"));
    newLoiterLapTimeItem->setText(VALUE_COLUMN, QString("%1 s").arg(calculateLapTime(waypoint), 0, 'f', 3));
}

void ReadonlyRoutesContent::addCustomProperties(QTreeWidgetItem* waypointItem, QJsonObject customPropertyExtension)
{
    if (customPropertyExtension.isEmpty()) {
        return;
    }

    foreach (QString key, customPropertyExtension.keys()) {
        QTreeWidgetItem* customProperty = new QTreeWidgetItem(waypointItem);
        customProperty->setText(KEY_COLUMN, key);
        QJsonValue value = customPropertyExtension[key];
        if (!value.isObject() && !value.isArray()) {
            customProperty->setText(VALUE_COLUMN, value.toString());
        } else if (value.isObject()) {
            addCustomProperties(customProperty, value.toObject());
        }
    }
}

QString ReadonlyRoutesContent::enumToString(const LmCdl::LoiterType loiterType)
{
    switch (loiterType) {
    case LmCdl::LoiterType::Invalid:
        return "Invalid";
    case LmCdl::LoiterType::Circular:
        return "Circular";
    case LmCdl::LoiterType::Racetrack:
        return "Racetrack";
    case LmCdl::LoiterType::Figure8:
        return "Figure8";
    case LmCdl::LoiterType::Hover:
        return "Hover";
    }
    return "Unknown";
}

QString ReadonlyRoutesContent::enumToString(const LmCdl::LoiterDirection loiterDirection)
{
    switch (loiterDirection) {
    case LmCdl::LoiterDirection::Clockwise:
        return "Clockwise";
    case LmCdl::LoiterDirection::CounterClockwise:
        return "CounterClockwise";
    }
    return "Unknown";
}
