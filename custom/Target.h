#pragma once

#include <QGeoCoordinate>
#include <QObject>
#include <QWidget>
#include <QImage>

#include <TargetWidget.h>

#include <LmCdl/I_GraphicsWidget.h>

struct Target : public QObject
{
    Q_OBJECT
public:
    Target(QGeoCoordinate location, TargetWidget* widget, QImage image)
    {
        Location = location;
        Widget   = widget;
        Image    = image;
    }

    Target(const Target& target)
    {
        Location = target.Location;
        Widget   = target.Widget;
        Image    = target.Image;
    }

    QGeoCoordinate Location;
    TargetWidget* Widget;
    QImage Image;
};