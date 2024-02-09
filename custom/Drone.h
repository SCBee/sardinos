#pragma once

#include <QGeoCoordinate>
#include <QImage>
#include <QObject>
#include <QTimer>

#include <LmCdl/I_Billboard.h>
#include <LmCdl/I_TrackVisualization.h>
#include <LmCdl/Speed.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/I_GraphicsWidget.h>

class Drone : public LmCdl::I_TrackVisualization
{
    Q_OBJECT
public:
    explicit Drone(volatile double& latitude,
                   volatile double& longitude,
                   volatile double& altitude,
                   volatile double& heading,
                   volatile double& speed,
                   volatile double& yaw,
                   volatile double& battery,
                   LmCdl::I_VcsiMapExtensionApi& mapApi);

    ~Drone();

    QGeoCoordinate location() const override;

    void setLocation(const QGeoCoordinate& location);

    LmCdl::WrappedAnglePlusMinusPi heading() const override;

    void setHeading(LmCdl::WrappedAnglePlusMinusPi heading);

    QColor color() const override;

    void setColor(QColor color);

    bool visible() const override;

    void setVisible(bool visible);

    void selected() override;

    void deselected() override;

    bool selectionEnabled() const override;

    void setSpeed(LmCdl::Speed speed);

    void setYaw(LmCdl::WrappedAnglePlusMinusPi yaw);

    void setBattery(double battery);

signals:

    void locationChanged(const QGeoCoordinate& location);

    void headingChanged(const LmCdl::WrappedAnglePlusMinusPi& heading);

    void colorChanged(const QColor& color);

    void visibleChanged(bool visible);

    void selectionEnabledChanged(bool selectionEnabled);

private:
    const QColor selectedColor_ = QColor("darkBlue");
    const QColor deselectedColor_ = QColor("blue");

    QGeoCoordinate location_;
    LmCdl::WrappedAnglePlusMinusPi heading_;
    LmCdl::Speed speed_;
    LmCdl::WrappedAnglePlusMinusPi yaw_;
    double battery_;
    QColor color_;
    QTimer* timer;
    bool visible_;

    LmCdl::I_GraphicsWidget* widget_;
};