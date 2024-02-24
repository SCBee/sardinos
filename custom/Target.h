#pragma once

#include <QGeoCoordinate>
#include <QImage>
#include <QObject>
#include <QWidget>

#include <LmCdl/I_GraphicsWidget.h>
#include <TargetWidget.h>

struct Target : public QObject
{
    Q_OBJECT
public:
    Target(QGeoCoordinate location, cv::Mat mat)
        : Location(location)
        , Mat(mat)
    {
    }

    Target(const Target& target)
        : Location(target.Location)
        , Mat(target.Mat)
    {
    }

    QGeoCoordinate Location;
    cv::Mat Mat;
};