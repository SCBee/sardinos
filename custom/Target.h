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
    Target(QGeoCoordinate location, TargetWidget* widget)
    {
        Location = location;
        Widget   = widget;
    }

    Target(const Target& target)
    {
        Location = target.Location;
        Widget   = target.Widget;
    }

    QGeoCoordinate Location;
    TargetWidget* Widget;
};