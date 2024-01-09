/**
 \file
 Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
 */

#include <SingleVehicleStatusDisplay.h>

#include <LmCdl/Distance.h>
#include <LmCdl/I_UnitPreferencesApi.h>
#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleLocation.h>
#include <LmCdl/I_VehicleVelocity.h>
#include <LmCdl/Speed.h>
#include <LmCdl/TabWidgetMetadata.h>
#include <QDebug>
#include <QGeoCoordinate>
#include <QSpacerItem>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <exception>
#include <stdexcept>

SingleVehicleStatusDisplay::SingleVehicleStatusDisplay(LmCdl::I_Vehicle& vehicle,
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::UniqueIdentifier vehicleId,
    LmCdl::I_UnitPreferencesApi& unitPreferencesApi)
    : vehicle_(vehicle)
    , unitPreferencesApi_(unitPreferencesApi)
{
    QWidget* mainWidget = createUi(vehicleExtensionApi, vehicleId);
    registerUi(vehicleExtensionApi, vehicleId, mainWidget);
    initializeDisplayContents(vehicleId);
}

QWidget* SingleVehicleStatusDisplay::createUi(
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::UniqueIdentifier vehicleId)
{
    auto centralWidget = new QWidget();
    centralWidget->setStyleSheet(QString("color: %1").arg(vehicleExtensionApi.vehicleColor(vehicleId).name()));

    auto centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setSpacing(0);

    idDisplay_ = new QLabel(centralWidget);
    centralLayout->addWidget(idDisplay_);

    latitudeLongitudeDisplay_ = new QLabel(centralWidget);
    centralLayout->addWidget(latitudeLongitudeDisplay_);

    altitudeDisplay_ = new QLabel(centralWidget);
    centralLayout->addWidget(altitudeDisplay_);

    airspeedDisplay_ = new QLabel(centralWidget);
    centralLayout->addWidget(airspeedDisplay_);

    auto verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    centralLayout->addItem(verticalSpacer);

    return centralWidget;
}

void SingleVehicleStatusDisplay::registerUi(
    LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::UniqueIdentifier vehicleId, QWidget* mainWidget)
{
    const LmCdl::TabWidgetMetadata metadata("Status");
    vehicleExtensionApi.addAvPanelTab(vehicleId, metadata, mainWidget);
}

void SingleVehicleStatusDisplay::initializeDisplayContents(LmCdl::UniqueIdentifier vehicleId)
{
    idDisplay_->setText(QString("Handle: %1").arg(QString::number(vehicleId.value())));

    const LmCdl::I_VehicleLocation& location = vehicle_.reportedLocation();
    connect(&location, &LmCdl::I_VehicleLocation::locationChanged, this,
        &SingleVehicleStatusDisplay::updateLatitudeLongitude);
    updateLatitudeLongitude();

    connect(&location, &LmCdl::I_VehicleLocation::barometricAltitudeMslChanged, this,
        &SingleVehicleStatusDisplay::updateAltitude);
    updateAltitude();

    connect(&vehicle_.reportedVelocity(), &LmCdl::I_VehicleVelocity::indicatedAirspeedChanged, this,
        &SingleVehicleStatusDisplay::updateAirspeed);
    connect(&unitPreferencesApi_.speedUnit(), &LmCdl::SpeedUnitPreference::changed, this,
        &SingleVehicleStatusDisplay::updateAirspeed);
    updateAirspeed();
}

void SingleVehicleStatusDisplay::updateLatitudeLongitude()
{
    const QGeoCoordinate location = vehicle_.reportedLocation().location();
    latitudeLongitudeDisplay_->setText(
        QString("Latitude: %1 degrees\nLongitude: %2 degrees").arg(location.latitude()).arg(location.longitude()));
}

void SingleVehicleStatusDisplay::updateAltitude()
{
    const LmCdl::Distance barometricAltitude = vehicle_.reportedLocation().barometricAltitudeMsl();
    altitudeDisplay_->setText(
        QString("Altitude (Baro): %1 m").arg(barometricAltitude.value(LmCdl::DistanceUnit::Meters)));
}

void SingleVehicleStatusDisplay::updateAirspeed()
{
    try {
        const LmCdl::SpeedUnit quantityUnit = unitPreferencesApi_.speedUnit().value();
        const LmCdl::Speed airspeed = vehicle_.reportedVelocity().indicatedAirspeed();
        const double speedAtProperPrecision = qRound(airspeed.value(quantityUnit));

        airspeedDisplay_->setText(
            QString("Airspeed: %1 %2").arg(speedAtProperPrecision).arg(speedUnitSymbol(quantityUnit)));
    } catch (const std::runtime_error& error) {
        qWarning() << error.what();
        airspeedDisplay_->setText(QString("Error: ") + error.what());
    }
}

QString SingleVehicleStatusDisplay::speedUnitSymbol(LmCdl::SpeedUnit unit) const
{
    switch (unit) {
    case LmCdl::SpeedUnit::Knots:
        return "kts";
    case LmCdl::SpeedUnit::MilesPerHour:
        return "mph";
    case LmCdl::SpeedUnit::FeetPerMinute:
        return "ft/min";
    case LmCdl::SpeedUnit::MetersPerSecond:
        return "m/sec";
    }

    return "????";
}
