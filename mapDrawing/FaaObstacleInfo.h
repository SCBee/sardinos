/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/Distance.h>
#include <QGeoCoordinate>
#include <QString>

class FaaObstacleInfo {
public:
    QGeoCoordinate location;
    QString descriptor;
    QString type;

    LmCdl::Distance aglHeight;
    LmCdl::Distance mslHeight;

    LmCdl::Distance horizontalAccuracy;
    LmCdl::Distance verticalAccuracy;

    bool verified;
    bool dismantled;
};
