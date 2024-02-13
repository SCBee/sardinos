#include <QCoreApplication>
#include <QTime>
#include <cmath>

#include <MathExt.h>
#include <qmath.h>

bool Comparator(const std::vector<QGeoCoordinate>& a,
                const std::vector<QGeoCoordinate>& b)
{
    if (a[1].longitude() != b[1].longitude()) {
        return a[1].longitude() < b[1].longitude();
    } else {
        return a[1].latitude() < b[1].latitude();
    }
}

void MathExt::cvhull(std::vector<std::vector<QGeoCoordinate>>& pois)
{
    double sumY = 0;  // latitude
    double sumX = 0;  // longitude
    for (const auto& poi : pois) {
        sumX += poi[0].longitude();  // long
        sumY += poi[0].latitude();  // lat
    }
    double comLat = sumY / (double)pois.size();  // y
    double comLong = sumX / (double)pois.size();  // x
    auto com = QGeoCoordinate(comLat, comLong);

    for (auto& poi : pois) {
        auto polarCoords = sqPolar(poi[0], com);
        auto tmp = QGeoCoordinate(polarCoords[1], polarCoords[0]);
        poi.push_back(tmp);
    }

    std::sort(pois.begin(), pois.end(), Comparator);
}

std::vector<double> MathExt::sqPolar(QGeoCoordinate& point, QGeoCoordinate& com)
{
    double angle = atan2(point.latitude() - com.latitude(),
                         point.longitude() - com.longitude());
    double distance = pow(point.longitude() - com.longitude(), 2)
        + pow(point.latitude() - com.latitude(), 2);

    return {angle, distance};
}

BoundingBox MathExt::findSmallestBoundingBox(
    const QList<LmCdl::VcsiIdentifiedPointOfInterest>& points)
{
    if (points.empty())
        return BoundingBox();

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

void MathExt::delay(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

double MathExt::getDistance(double lat1, double lon1, double lat2, double lon2)
{
    return getDistance(QGeoCoordinate(lat1, lon1), QGeoCoordinate(lat2, lon2));
}

double MathExt::getDistance(QGeoCoordinate c1, QGeoCoordinate c2)
{
    const double R = 6378.137;
    double dLat = (c2.latitude() * M_PI / 180) - (c1.latitude() * M_PI / 180);
    double dLon = (c2.longitude() * M_PI / 180) - (c1.longitude() * M_PI / 180);
    double a = sin(dLat / 2) * sin(dLat / 2)
        + cos(c1.latitude() * M_PI / 180) * cos(c2.latitude() * M_PI / 180)
            * sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = R * c;
    return d * 1000;  // meters
}

bool MathExt::isVertical(BoundingBox box)
{
    if (MathExt().getDistance(box.SE, box.SW)
        > MathExt().getDistance(box.SE, box.NE))
        return false;
    return true;
}