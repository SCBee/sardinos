/**
\file
Copyright (c) 2020 Lockheed Martin Corp.  All rights reserved.
*/

#include <ExamplePayloadDomain.h>

#include <ExamplePayloadProjectedFootprintDrawing.h>
#include <ExamplePayloadStarepointDrawing.h>
#include <LmCdl/GroundElevation.h>
#include <LmCdl/I_GroundElevationApi.h>

namespace {
QGeoCoordinate someLocationInHawaii() { return QGeoCoordinate(21.40375, -157.9866833, 1000); }
}

ExamplePayloadDomain::ExamplePayloadDomain(LmCdl::I_GroundElevationApi& groundElevationApi)
    : projectedFootprintDrawing_(new ExamplePayloadProjectedFootprintDrawing)
    , starepointNearFootprint_(new ExamplePayloadStarepointDrawing())
{
    projectedFootprintDrawing_->setVisible(true);
    projectedFootprintDrawing_->setCameraLocation(someLocationInHawaii());
    projectedFootprintDrawing_->setCameraOrientation(QQuaternion::fromEulerAngles(-90, 0, 0));
    projectedFootprintDrawing_->setCameraHorizontalFieldOfView(
        LmCdl::WrappedAnglePlusMinusPi(16, LmCdl::AngleUnit::Degrees));
    projectedFootprintDrawing_->setCameraVerticalFieldOfView(
        LmCdl::WrappedAnglePlusMinusPi(9, LmCdl::AngleUnit::Degrees));
    projectedFootprintDrawing_->setColor(QColor("red"));
    projectedFootprintDrawings_ << projectedFootprintDrawing_.data();

    QGeoCoordinate starepointLocation = someLocationInHawaii();
    starepointLocation.setAltitude(groundElevationApi.lookupGroundElevationSynchronously(starepointLocation)
                                       .altitudeMsl()
                                       .value(LmCdl::DistanceUnit::Meters));
    starepointNearFootprint_->setLocation(starepointLocation);
    starepointNearFootprint_->setColor(QColor("#ff69b4"));
    starepointNearFootprint_->setVisible(true);
    starepointDrawings_ << starepointNearFootprint_.data();

    lowerStarepoint_.reset(new ExamplePayloadStarepointDrawing());
    lowerStarepoint_->setLocation(QGeoCoordinate(21.3163667, -158.0055619, 500));
    lowerStarepoint_->setColor(Qt::black);
    lowerStarepoint_->setVisible(true);
    lowerStarepoint_->setRelativeZOrder(2);
    starepointDrawings_ << lowerStarepoint_.data();

    higherStarepoint_.reset(new ExamplePayloadStarepointDrawing());
    higherStarepoint_->setLocation(lowerStarepoint_->location());
    higherStarepoint_->setColor(Qt::white);
    higherStarepoint_->setVisible(true);
    higherStarepoint_->setRelativeZOrder(4);
    starepointDrawings_ << higherStarepoint_.data();

    connect(&higherStarepointVisibilityToggleTimer_, &QTimer::timeout, this,
        [this]() { higherStarepoint_->setVisible(!higherStarepoint_->visible()); });
    higherStarepointVisibilityToggleTimer_.start(2500);
}

ExamplePayloadDomain::~ExamplePayloadDomain() { }

QList<LmCdl::I_PayloadProjectedFootprint*> ExamplePayloadDomain::projectedFootprintDrawings() const
{
    return projectedFootprintDrawings_;
}

QList<LmCdl::I_PayloadStarepoint*> ExamplePayloadDomain::starepointsDrawings() const { return starepointDrawings_; }
