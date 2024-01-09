/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <ExampleTrackDomain.h>

#include <LmCdl/AngleUnit.h>
#include <LmCdl/WrappedAnglePlusMinusPi.h>
#include <QColor>

namespace {
const double LATITUDE_IN_HAWAII = 21.40375;
const double LONGITUDE_IN_HAWAII = -157.9866833;
}

ExampleTrackDomain::ExampleTrackDomain()
    : track1_(new ExampleTrackVisualization)
    , track2_(new ExampleTrackVisualization)
    , track3_(new ExampleTrackVisualization)
    , allTracks_()
{
    setupTrackForHawaii(*track1_, 1);
    setupTrackForHawaii(*track2_, 2);
    setupTrackForHawaii(*track3_, 3);
}

ExampleTrackDomain::~ExampleTrackDomain() { }

QList<ExampleTrackVisualization*> ExampleTrackDomain::tracks() const { return allTracks_; }

void ExampleTrackDomain::setupTrackForHawaii(ExampleTrackVisualization& track, int id)
{
    track.setLocation(QGeoCoordinate(LATITUDE_IN_HAWAII, LONGITUDE_IN_HAWAII + (id / 10.0), 7000));
    track.setHeading(LmCdl::WrappedAnglePlusMinusPi(0, LmCdl::AngleUnit::Degrees));
    track.setColor(Qt::blue);
    allTracks_.append(&track);
}
