#pragma once

#include <QCoreApplication>
#include <QGeoCoordinate>
#include <QList>
#include <QTime>
#include <qmath.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>

#include "BoundingBox.h"
#include "Waypoint/Waypoint.h"
#include "Waypoint/WaypointConnector.h"
#undef min
#undef max

class sardinos
{
    static inline const double SCANWIDTHMETERS   = 50.0;
    static inline const double MAXDISTANCEMETERS = 200000.0;
    static inline const double R                 = 6378.137;

public:
    static void delay(const int& ms)
    {
        QTime dieTime = QTime::currentTime().addMSecs(ms);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    static bool Comparator(const std::vector<QGeoCoordinate>& a,
                           const std::vector<QGeoCoordinate>& b)
    {
        if (a[1].longitude() != b[1].longitude()) {
            return a[1].longitude() < b[1].longitude();
        } else {
            return a[1].latitude() < b[1].latitude();
        }
    }

    static std::vector<double> sqPolar(const QGeoCoordinate& point,
                                       const QGeoCoordinate& com)
    {
        double angle    = atan2(point.latitude() - com.latitude(),
                             point.longitude() - com.longitude());
        double distance = pow(point.longitude() - com.longitude(), 2)
            + pow(point.latitude() - com.latitude(), 2);

        return {angle, distance};
    }

    static void cvhull(std::vector<std::vector<QGeoCoordinate>>& pois)
    {
        double sumY = 0;  // latitude
        double sumX = 0;  // longitude
        for (const auto& poi : pois) {
            sumX += poi[0].longitude();  // long
            sumY += poi[0].latitude();  // lat
        }
        double comLat  = sumY / (double)pois.size();  // y
        double comLong = sumX / (double)pois.size();  // x
        auto com       = QGeoCoordinate(comLat, comLong);

        for (auto& poi : pois) {
            auto polarCoords = sqPolar(poi[0], com);
            auto tmp         = QGeoCoordinate(polarCoords[1], polarCoords[0]);
            poi.push_back(tmp);
        }

        std::sort(pois.begin(), pois.end(), Comparator);
    }

    static BoundingBox findSmallestBoundingBox(
        const QList<LmCdl::VcsiIdentifiedPointOfInterest>& points)
    {
        if (points.empty())
            return {};

        QGeoCoordinate southwest, northeast, southeast, northwest;
        southwest = northeast = southeast = northwest =
            points[0].pointOfInterest().location();

        for (const auto& point : points) {
            southwest.setLatitude(
                std::min(southwest.latitude(),
                         point.pointOfInterest().location().latitude()));
            southwest.setLongitude(
                std::min(southwest.longitude(),
                         point.pointOfInterest().location().longitude()));

            northeast.setLatitude(
                std::max(northeast.latitude(),
                         point.pointOfInterest().location().latitude()));
            northeast.setLongitude(
                std::max(northeast.longitude(),
                         point.pointOfInterest().location().longitude()));
        }

        southeast.setLatitude(southwest.latitude());
        southeast.setLongitude(northeast.longitude());

        northwest.setLatitude(northeast.latitude());
        northwest.setLongitude(southwest.longitude());

        return {southwest, northwest, southeast, northeast};
    }

    static double getDistance(const QGeoCoordinate& c1,
                              const QGeoCoordinate& c2)
    {
        double dLat =
            (c2.latitude() * M_PI / 180) - (c1.latitude() * M_PI / 180);
        double dLon =
            (c2.longitude() * M_PI / 180) - (c1.longitude() * M_PI / 180);
        double a = sin(dLat / 2) * sin(dLat / 2)
            + cos(c1.latitude() * M_PI / 180) * cos(c2.latitude() * M_PI / 180)
                * sin(dLon / 2) * sin(dLon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        double d = R * c;
        return d * 1000;  // meters
    }

    static double getDistance(double lat1,
                              double lon1,
                              double lat2,
                              double lon2)
    {
        return getDistance(QGeoCoordinate(lat1, lon1),
                           QGeoCoordinate(lat2, lon2));
    }

    static bool isVertical(const BoundingBox& box)
    {
        if (getDistance(box.SE, box.SW) > getDistance(box.SE, box.NE))
            return false;
        return true;
    }

    static QList<QGeoCoordinate> getVerticalFlightPath(
        const BoundingBox& missionBounds)
    {
        auto longSpreadMeters = getDistance(missionBounds.NE, missionBounds.NW);

        auto longSpread =
            abs(missionBounds.eastBound() - missionBounds.westBound());

        auto steps = std::max(1.0, longSpreadMeters / SCANWIDTHMETERS);

        auto stepLongitude = longSpread / steps;

        auto currentLocation = missionBounds.SW;

        currentLocation.setLongitude(currentLocation.longitude()
                                     + (stepLongitude / 2.0));

        auto wayPoints = QList<QGeoCoordinate>();

        auto goingUp = true;

        while (currentLocation.longitude() < missionBounds.eastBound()) {
            wayPoints.append(currentLocation);

            if (goingUp
                && currentLocation.latitude() == missionBounds.southBound())
            {
                currentLocation.setLatitude(missionBounds.northBound());
                continue;
            }

            if (goingUp
                && currentLocation.latitude() == missionBounds.northBound())
            {
                currentLocation.setLongitude(currentLocation.longitude()
                                             + stepLongitude);
                goingUp = false;
                continue;
            }

            if (!goingUp
                && currentLocation.latitude() == missionBounds.northBound())
            {
                currentLocation.setLatitude(missionBounds.southBound());
                continue;
            }

            if (!goingUp
                && currentLocation.latitude() == missionBounds.southBound())
            {
                currentLocation.setLongitude(currentLocation.longitude()
                                             + stepLongitude);
                goingUp = true;
                continue;
            }
        }

        return wayPoints;
    }

    static QList<QGeoCoordinate> getHorizontalFlightPath(
        const BoundingBox& missionBounds)
    {
        auto latSpreadMeters = getDistance(missionBounds.NE, missionBounds.SE);

        auto latSpread =
            abs(missionBounds.northBound() - missionBounds.southBound());

        auto steps = std::max(1.0, latSpreadMeters / SCANWIDTHMETERS);

        auto stepLatitude = latSpread / steps;

        auto currentLocation = missionBounds.SW;

        currentLocation.setLatitude(currentLocation.latitude()
                                    + (stepLatitude / 2.0));

        auto wayPoints = QList<QGeoCoordinate>();

        auto goingRight = true;

        while (currentLocation.latitude() < missionBounds.northBound()) {
            wayPoints.append(currentLocation);

            if (goingRight
                && currentLocation.longitude() == missionBounds.westBound())
            {
                currentLocation.setLongitude(missionBounds.eastBound());
                continue;
            }

            if (goingRight
                && currentLocation.longitude() == missionBounds.eastBound())
            {
                currentLocation.setLatitude(currentLocation.latitude()
                                            + stepLatitude);
                goingRight = false;
                continue;
            }

            if (!goingRight
                && currentLocation.longitude() == missionBounds.eastBound())
            {
                currentLocation.setLongitude(missionBounds.westBound());
                continue;
            }

            if (!goingRight
                && currentLocation.longitude() == missionBounds.westBound())
            {
                currentLocation.setLatitude(currentLocation.latitude()
                                            + stepLatitude);
                goingRight = true;
                continue;
            }
        }

        return wayPoints;
    }

    static QList<QGeoCoordinate> getPath(const BoundingBox& missionBounds)
    {
        if (isVertical(missionBounds))
            return getVerticalFlightPath(missionBounds);
        else
            return getHorizontalFlightPath(missionBounds);
    }

    static bool AreSame(double a, double b)
    {
        return fabs(a - b) < std::numeric_limits<double>::epsilon();
    }

    static QList<QGeoCoordinate> getCheesePath(const BoundingBox& missionBounds)
    {
        std::vector<double> lats = {50.9986, 50.9986, 50.9981, 50.9981, 50.9981, 50.9981, 50.9976, 50.9976, 50.9976, 50.9976, 50.9976, 50.9976, 50.9976, 50.9971, 50.9971, 50.9971, 50.9971, 50.9971, 50.9971, 50.9971, 50.9971, 50.9971, 50.9966, 50.9966, 50.9966, 50.9966, 50.9966, 50.9966, 50.9966, 50.9966, 50.9966, 50.9966, 50.9961, 50.9961, 50.9961, 50.9961, 50.9961, 50.9961, 50.9961, 50.9961, 50.9961, 50.9961, 50.9961, 50.9961, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9956, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9951, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9946, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9941, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9936, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9931, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9926, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9921, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9916, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9911, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9906, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9901, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9896, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9891, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9886, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9881, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9876, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9871, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9866, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9861, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856, 50.9856};
        std::vector<double> lons = {-114.01025, -114.00975, -114.00925, -114.00975, -114.01025, -114.01075, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00475, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01375, -114.01425, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00375, -114.00425, -114.00475, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01375, -114.01425, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00375, -114.00325, -114.00275, -114.00325, -114.00375, -114.00425, -114.00475, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01375, -114.01425, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00375, -114.00325, -114.00275, -114.00225, -114.00275, -114.00325, -114.00375, -114.00425, -114.00475, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01375, -114.01425, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00375, -114.00325, -114.00275, -114.00225, -114.00175, -114.00125, -114.00175, -114.00225, -114.00275, -114.00325, -114.00375, -114.00425, -114.00475, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01375, -114.01425, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00375, -114.00325, -114.00275, -114.00225, -114.00175, -114.00125, -114.00075, -114.00025, -114.00075, -114.00125, -114.00175, -114.00225, -114.00275, -114.00325, -114.00375, -114.00425, -114.00475, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01375, -114.01425, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00375, -114.00325, -114.00275, -114.00225, -114.00175, -114.00125, -114.00075, -114.00025, -113.99975, -113.99975, -114.00025, -114.00075, -114.00125, -114.00175, -114.00225, -114.00275, -114.00325, -114.00375, -114.00425, -114.00475, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01375, -114.01425, -114.01475, -114.01525, -114.01475, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00375, -114.00325, -114.00275, -114.00225, -114.00175, -114.00125, -114.00075, -114.00025, -113.99975, -113.99975, -114.00025, -114.00075, -114.00125, -114.00175, -114.00225, -114.00275, -114.00325, -114.00375, -114.00425, -114.00475, -114.00525, -114.00575, -114.00625, -114.00675, -114.00725, -114.00775, -114.00825, -114.00875, -114.00925, -114.00975, -114.01025, -114.01075, -114.01125, -114.01175, -114.01225, -114.01275, -114.01325, -114.01375, -114.01425, -114.01475, -114.01525, -114.01525, -114.01475, -114.01425, -114.01375, -114.01325, -114.01275, -114.01225, -114.01175, -114.01125, -114.01075, -114.01025, -114.00975, -114.00925, -114.00875, -114.00825, -114.00775, -114.00725, -114.00675, -114.00625, -114.00575, -114.00525, -114.00475, -114.00425, -114.00375, -114.00325, -114.00275, -114.00225, -114.00175, -114.00125, -114.00075, -114.00025, -113.99975};
        
        QList<QGeoCoordinate> result;

        for (auto i = 1; i < lats.size() - 1; i++){
            if (AreSame(lats[i-1], lats[i]) && AreSame( lats[i], lats[i+1])) continue;
            result.append(QGeoCoordinate(lats[i], lons[i], 1120));
        }

        result.append(QGeoCoordinate(lats[lats.size() - 1], lons[lons.size() - 1], 1120));

        return result;
    }

    static bool canFly(const QList<Waypoint*>& waypoints)
    {
        auto sum = 0.0;

        for (auto i = 0; i < waypoints.size() - 1; i++) {
            sum += getDistance(waypoints[i]->location(),
                               waypoints[i + 1]->location());
        }

        return sum <= MAXDISTANCEMETERS;
    }

    static QGeoCoordinate getLocation(const double lat,
                                      const double lon,
                                      const double alt,
                                      double d,  // meters
                                      double a)  // degrees
    {
        a *= M_PI / 180;  // to rads
        d /= 1000.0;  // to kms
        auto lat1 = lat * (M_PI / 180);
        auto lon1 = lon * (M_PI / 180);

        auto lat2 =
            asin(sin(lat1) * cos(d / R) + cos(lat1) * sin(d / R) * cos(a));
        auto lon2 = lon1
            + atan2(sin(a) * sin(d / R) * cos(lat1),
                    cos(d / R) - sin(lat1) * sin(lat2));

        // Normalize longitude to the range -180 to 180 degrees
        lon2 = fmod(lon2 + 3 * M_PI, 2 * M_PI) - M_PI;

        return {lat2 * (180 / M_PI), lon2 * (180 / M_PI), alt};
    }
};
