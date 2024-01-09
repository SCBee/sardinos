/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <QFile>
#include <QList>
#include <QString>

#include <LmCdl/Distance.h>
#include <QGeoCoordinate>

#include <FaaObstacleInfo.h>

class FaaObstacleSource {
public:
    explicit FaaObstacleSource(const QString& digitalObstacleFilePath);
    virtual ~FaaObstacleSource();

    QList<FaaObstacleInfo> obstaclesNearLocation(const QGeoCoordinate& location,
        const LmCdl::Distance range = LmCdl::Distance(100000, LmCdl::DistanceUnit::Meters));
    QList<FaaObstacleInfo> obstacles();

private:
    void skipPastHeader(QFile& file);

    bool loadObstacle(const QString& line, FaaObstacleInfo& info);
    bool loadObstacleVerificationStatus(const QString& line, FaaObstacleInfo& info);
    bool loadObstaclePosition(const QString& line, FaaObstacleInfo& info);
    bool loadObstacleAltitude(const QString& line, FaaObstacleInfo& info);
    bool loadObstacleAltitudeHorizontalAccuracy(const QString& line, FaaObstacleInfo& info);
    bool loadObstacleAltitudeVerticalAccuracy(const QString& line, FaaObstacleInfo& info);
    bool loadObstacleActionStatus(const QString& line, FaaObstacleInfo& info);

private:
    QString digitalObstacleFilePath_;
};
