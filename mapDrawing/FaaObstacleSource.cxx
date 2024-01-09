/**
\file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#include <math.h>

#include <QFile>

#include <LmCdl/Distance.h>
#include <LmCdl/WrappedAnglePlusMinusPi.h>
#include <QGeoCoordinate>

#include <FaaObstacleSource.h>

FaaObstacleSource::FaaObstacleSource(const QString& digitalObstacleFilePath)
    : digitalObstacleFilePath_(digitalObstacleFilePath)
{
}

FaaObstacleSource::~FaaObstacleSource() { }

QList<FaaObstacleInfo> FaaObstacleSource::obstacles()
{
    QList<FaaObstacleInfo> obstacleList;

    QFile file(digitalObstacleFilePath_);
    if (file.open(QFile::ReadOnly)) {
        skipPastHeader(file);

        while (!file.atEnd()) {
            FaaObstacleInfo obstacle;
            if (loadObstacle(file.readLine(), obstacle)) {
                obstacleList.append(obstacle);
            }
        }
    } else {
        qDebug("Couldn't open Digital Obstacle File: %s", qPrintable(digitalObstacleFilePath_));
    }
    return obstacleList;
}

bool FaaObstacleSource::loadObstacle(const QString& line, FaaObstacleInfo& info)
{
    if (!loadObstacleVerificationStatus(line, info)) {
        return false;
    }
    if (!loadObstaclePosition(line, info)) {
        return false;
    }
    if (!loadObstacleActionStatus(line, info)) {
        return false;
    }
    if (!loadObstacleAltitude(line, info)) {
        return false;
    }

    bool horizontalAccuracyLoaded = loadObstacleAltitudeHorizontalAccuracy(line, info);
    bool verticalAccuracyLoaded = loadObstacleAltitudeVerticalAccuracy(line, info);
    if (!info.dismantled && (!horizontalAccuracyLoaded || !verticalAccuracyLoaded)) {
        return false;
    }

    return true;
}

void FaaObstacleSource::skipPastHeader(QFile& file)
{
    file.readLine(); // CURRENCY DATE = %1
    file.readLine(); // Title Line 1
    file.readLine(); // Title Line 2
    file.readLine(); // ------------------
}

bool FaaObstacleSource::loadObstacleVerificationStatus(const QString& line, FaaObstacleInfo& info)
{
    QChar verificationStatus = line.at(10);
    if (verificationStatus.toLower() == QChar('o')) {
        info.verified = true;
    } else if (verificationStatus.toLower() == QChar('u')) {
        info.verified = false;
    } else {
        return false;
    }

    return true;
}

bool FaaObstacleSource::loadObstaclePosition(const QString& line, FaaObstacleInfo& info)
{
    bool ok = false;

    int latitudeDeg = line.mid(35, 2).toInt(&ok);
    if (!ok)
        return false;

    int latitudeMin = line.mid(38, 2).toInt(&ok);
    if (!ok)
        return false;

    double latitudeSec = line.mid(41, 5).toDouble(&ok);
    if (!ok)
        return false;

    QChar latitudeHemisphere = line.at(46);

    double latitude = latitudeDeg + (latitudeMin / 60.0) + (latitudeSec / 3600.0);
    if (latitudeHemisphere.toLower() == QChar('s')) {
        latitude *= -1;
    }

    int longitudeDeg = line.mid(48, 3).toInt(&ok);
    if (!ok)
        return false;

    int longitudeMin = line.mid(52, 2).toInt(&ok);
    if (!ok)
        return false;

    double longitudeSec = line.mid(55, 5).toDouble(&ok);
    if (!ok)
        return false;

    QChar longitudeHemisphere = line.at(60);

    double longitude = longitudeDeg + (longitudeMin / 60.0) + (longitudeSec / 3600.0);
    if (longitudeHemisphere.toLower() == QChar('w')) {
        longitude *= -1;
    }

    info.location = QGeoCoordinate(latitude, longitude);

    return true;
}

bool FaaObstacleSource::loadObstacleAltitude(const QString& line, FaaObstacleInfo& info)
{
    bool ok = false;

    int altitudeAglInFeet = line.mid(83, 5).toInt(&ok);
    if (!ok)
        return false;
    info.aglHeight = LmCdl::Distance(altitudeAglInFeet, LmCdl::DistanceUnit::Feet);

    int altitudeMslInFeet = line.mid(89, 5).toInt(&ok);
    if (!ok)
        return false;
    info.mslHeight = LmCdl::Distance(altitudeMslInFeet, LmCdl::DistanceUnit::Feet);

    return true;
}

bool FaaObstacleSource::loadObstacleAltitudeHorizontalAccuracy(const QString& line, FaaObstacleInfo& info)
{
    bool ok = false;

    int horizontalAccuracyCode = QString(line.at(97)).toInt(&ok);
    if (!ok)
        return false;

    double horizontalAccuracyInFeet = -1;

    bool isAccuracyKnown = true;
    switch (horizontalAccuracyCode) {
    case 1:
        horizontalAccuracyInFeet = 20;
        break;
    case 2:
        horizontalAccuracyInFeet = 50;
        break;
    case 3:
        horizontalAccuracyInFeet = 100;
        break;
    case 4:
        horizontalAccuracyInFeet = 250;
        break;
    case 5:
        horizontalAccuracyInFeet = 500;
        break;
    case 6:
        horizontalAccuracyInFeet = 1000;
        break;
    case 7:
        horizontalAccuracyInFeet = 3038;
        break;
    case 8:
        horizontalAccuracyInFeet = 6076;
        break;
    case 9:
    default:
        isAccuracyKnown = false;
    }

    if (isAccuracyKnown) {
        info.horizontalAccuracy = LmCdl::Distance(horizontalAccuracyInFeet, LmCdl::DistanceUnit::Feet);
        return true;
    }
    return false;
}

bool FaaObstacleSource::loadObstacleAltitudeVerticalAccuracy(const QString& line, FaaObstacleInfo& info)
{
    QChar verticalAccuracyCode = line.at(99).toLower();

    bool isAccuracyKnown = true;
    double verticalAccuracyInFeet = -1;

    if (verticalAccuracyCode == QChar('a')) {
        verticalAccuracyInFeet = 3;
    } else if (verticalAccuracyCode == QChar('b')) {
        verticalAccuracyInFeet = 10;
    } else if (verticalAccuracyCode == QChar('c')) {
        verticalAccuracyInFeet = 20;
    } else if (verticalAccuracyCode == QChar('d')) {
        verticalAccuracyInFeet = 50;
    } else if (verticalAccuracyCode == QChar('e')) {
        verticalAccuracyInFeet = 125;
    } else if (verticalAccuracyCode == QChar('f')) {
        verticalAccuracyInFeet = 250;
    } else if (verticalAccuracyCode == QChar('g')) {
        verticalAccuracyInFeet = 500;
    } else if (verticalAccuracyCode == QChar('h')) {
        verticalAccuracyInFeet = 1000;
    } else {
        isAccuracyKnown = false;
    }

    if (isAccuracyKnown) {
        info.verticalAccuracy = LmCdl::Distance(verticalAccuracyInFeet, LmCdl::DistanceUnit::Feet);
        return true;
    }

    return false;
}

bool FaaObstacleSource::loadObstacleActionStatus(const QString& line, FaaObstacleInfo& info)
{
    QChar actionStatus = line.at(112);
    if (actionStatus.toLower() == QChar('d')) {
        info.dismantled = true;
    } else {
        info.dismantled = false;
    }
    return true;
}
