/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#include <BatteryContent.h>

#include <LmCdl/I_QmlApi.h>
#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/I_VehicleFuel.h>
#include <LmCdl/Percentage.h>
#include <LmCdl/TabWidgetMetadata.h>
#include <QLabel>
#include <QQuickItem>
#include <QQuickView>
#include <QVariant>

BatteryContent::BatteryContent(LmCdl::I_VehicleCollectionApi& vehicleCollectionApi,
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::I_QmlApi& qmlApi)
    : vehicleCollectionApi_(vehicleCollectionApi)
    , vehicleExtensionApi_(vehicleExtensionApi)
    , qmlApi_(qmlApi)
{
    connect(&vehicleCollectionApi_, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this, &BatteryContent::addNewVehicle);
    connect(&vehicleCollectionApi_, &LmCdl::I_VehicleCollectionApi::vehicleAboutToBeRemoved, this,
        &BatteryContent::removeVehicle);
}

void BatteryContent::addNewVehicle(LmCdl::UniqueIdentifier vehicleId)
{
    QQuickView* view = new QQuickView(&qmlApi_.qmlEngine(), nullptr);
    view->setSource(QUrl("qrc:///battery/batteryIndicator.qml"));
    QWidget* container = QWidget::createWindowContainer(view);

    const LmCdl::TabWidgetMetadata metadata("Battery");
    vehicleExtensionApi_.addAvPanelTab(vehicleId, metadata, container);
    vehicles_.append(vehicleId);

    vehicleGauges_.insert(vehicleId, view->rootObject());

    auto updateBattery = [this, vehicleId]() { this->updateBatteryPercentage(vehicleId); };
    connect(&vehicleCollectionApi_.vehicle(vehicleId).reportedFuel(), &LmCdl::I_VehicleFuel::fuelLevelChanged,
        updateBattery);
    updateBattery();
}

void BatteryContent::removeVehicle(LmCdl::UniqueIdentifier vehicleId) { vehicles_.removeAll(vehicleId); }

void BatteryContent::updateBatteryPercentage(LmCdl::UniqueIdentifier vehicleId)
{
    QMetaObject::invokeMethod(
        vehicleGauges_[vehicleId], "setValue", Q_ARG(QVariant, vehicleFuelLevel(vehicleId).toUInt()));
}

LmCdl::Percentage BatteryContent::vehicleFuelLevel(LmCdl::UniqueIdentifier vehicleId)
{
    return vehicleCollectionApi_.vehicle(vehicleId).reportedFuel().fuelLevel();
}
