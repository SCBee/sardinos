/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <GdtWidgets.h>

#include <LmCdl/Distance.h>
#include <LmCdl/I_GdtApi.h>
#include <LmCdl/I_VehicleLocation.h>
#include <LmCdl/WrappedAnglePlusMinusPi.h>
#include <QGeoCoordinate>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QtMath>

GdtWidgets::GdtWidgets(
    LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VehicleLocation& vehicleLocationApi, LmCdl::I_GdtApi& gdtApi)
    : vehicleId_(vehicleId)
    , gdtApi_(gdtApi)

    , graphicsWidgetContents_(new QWidget())

    , gdtLatitude_(new QLabel(graphicsWidgetContents_.data()))
    , gdtLongitude_(new QLabel(graphicsWidgetContents_.data()))
    , gdtHorizontalRange_(new QLabel(graphicsWidgetContents_.data()))
    , gdtSlantRange_(new QLabel(graphicsWidgetContents_.data()))
    , bearingToGdt_(new QLabel(graphicsWidgetContents_.data()))
{
    QVBoxLayout* layout = new QVBoxLayout(graphicsWidgetContents_.data());
    layout->addWidget(gdtLatitude_);
    layout->addWidget(gdtLongitude_);
    layout->addWidget(gdtHorizontalRange_);
    layout->addWidget(gdtSlantRange_);
    layout->addWidget(bearingToGdt_);

    connect(&vehicleLocationApi, &LmCdl::I_VehicleLocation::locationChanged, this, &GdtWidgets::refreshContents);
    connect(&gdtApi_, &LmCdl::I_GdtApi::gdtLocationChanged, this, &GdtWidgets::refreshContents);
    refreshContents();
}

GdtWidgets::~GdtWidgets() { delete graphicsWidgetContents_; }

QWidget& GdtWidgets::graphicsWidgetContents() { return *graphicsWidgetContents_; }

void GdtWidgets::refreshContents()
{
    QGeoCoordinate gdtLocation = gdtApi_.gdtLocation();
    LmCdl::Distance horizontalRangeToGdt = gdtApi_.groundRangeToVehicle(vehicleId_);
    LmCdl::Distance slantRangeToGdt = gdtApi_.slantRangeToVehicle(vehicleId_);
    LmCdl::WrappedAnglePlusMinusPi gdtBearingToVehicle = gdtApi_.bearingTo(vehicleId_);
    double flippedBearingInRadians = gdtBearingToVehicle.value(LmCdl::AngleUnit::Radians) + M_PI;
    LmCdl::WrappedAnglePlusMinusPi vehicleBearingToGdt(flippedBearingInRadians, LmCdl::AngleUnit::Radians);

    gdtLatitude_->setText(QString("GDT Latitude: %1 degrees").arg(gdtLocation.latitude()));
    gdtLongitude_->setText(QString("GDT Longitude: %1 degrees").arg(gdtLocation.longitude()));
    gdtHorizontalRange_->setText(
        QString("GDT HRange: %1m").arg(horizontalRangeToGdt.value(LmCdl::DistanceUnit::Meters)));
    gdtSlantRange_->setText(QString("GDT SRange: %1m").arg(slantRangeToGdt.value(LmCdl::DistanceUnit::Meters)));
    bearingToGdt_->setText(QString("GDT Bearing: %1 rad").arg(vehicleBearingToGdt.value(LmCdl::AngleUnit::Radians)));
}
