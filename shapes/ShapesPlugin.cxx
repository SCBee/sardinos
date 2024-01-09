/**
\file
Copyright (c) 2016 Lockheed Martin Canada Inc. All rights reserved.
*/

#include <ShapesPlugin.h>

#include <LmCdl/I_PanterraPolygon.h>
#include <LmCdl/I_PanterraPolygonApi.h>
#include <LmCdl/I_PanterraPublicApi.h>
#include <LmCdl/I_PanterraShape.h>
#include <LmCdl/I_PanterraShapeApi.h>
#include <LmCdl/PluginCapabilityIdentifier.h>
#include <LmCdl/PluginRequirement.h>
#include <QColor>
#include <QList>
#include <QtPlugin>

const int TIME_TO_TOGGLE_VISIBILITY = 1000;
const double anonymousLatitudeInHawaii = 0.347926538;
const double anonymousLongitudeInHawaii = -2.714990551;

ShapesPlugin::ShapesPlugin()
    : panterraApi_(NULL)
    , sphere_(NULL)
    , cylinder_(NULL)
{
    connect(&timerToRemoveSphere_, SIGNAL(timeout()), this, SLOT(removeSphere()));
    connect(&timerToCreateSphere_, SIGNAL(timeout()), this, SLOT(createSphere()));
    timerToRemoveSphere_.setSingleShot(true);
    timerToCreateSphere_.setSingleShot(true);
}

ShapesPlugin::~ShapesPlugin() { }

QList<LmCdl::PluginRequirement> ShapesPlugin::requiredApis() const
{
    return { LmCdl::PluginRequirement(PANTERRA_PUBLIC_API_CAPABILITY_NAME, 1, 0, 0) };
}

LmCdl::PluginCapabilityIdentifier ShapesPlugin::providedApi() const
{
    return LmCdl::PluginCapabilityIdentifier::noCapability();
}

bool ShapesPlugin::setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api)
{
    bool capabilityFound = false;
    if (id.capabilityName() == PANTERRA_PUBLIC_API_CAPABILITY_NAME) {
        panterraApi_ = dynamic_cast<LmCdl::I_PanterraPublicApi*>(api);
        capabilityFound = true;
    }

    createStaticShapes();
    return capabilityFound;
}

QObject* ShapesPlugin::getProvidedApi() { return nullptr; }

bool ShapesPlugin::isFullyInitialized() const { return (panterraApi_); }

void ShapesPlugin::createSphere()
{
    if (isFullyInitialized()) {
        double latitudeRadians = anonymousLatitudeInHawaii;
        double longitudeRadians = anonymousLongitudeInHawaii;
        double altitudeMeters = 20000.0;
        double radiusMeters = 20000.0;
        sphere_ = &panterraApi_->shapeApi().createShape(
            panterraApi_->polygonApi().createSphere(latitudeRadians, longitudeRadians, altitudeMeters, radiusMeters));
        startTimerToRemoveSphere();
    }
}

void ShapesPlugin::removeSphere()
{
    if (isFullyInitialized()) {
        panterraApi_->shapeApi().removeShape(*sphere_);
        startTimerToCreateSphere();
    }
}

void ShapesPlugin::createStaticShapes()
{
    createSphere();
    createCylinder();
    createRectangularPrism();
    createVerticalRectangularPrismHighToLow();
    createVerticalRectangularPrismLowToHigh();
    createHorizontalRectangularPrismNorthToSouth();
    createHorizontalRectangularPrismSouthToNorth();
    createHorizontalRectangularPrismEastToWest();
    createHorizontalRectangularPrismWestToEast();
    createDisk();
    createArc();
}

void ShapesPlugin::createCylinder()
{
    double startPointLatitudeRadians = anonymousLatitudeInHawaii;
    double startPointLongitudeRadians = anonymousLongitudeInHawaii - 0.01;
    double startPointAltitudeMeters = 40000.0;
    double endPointLatitudeRadians = anonymousLatitudeInHawaii;
    double endPointLongitudeRadians = anonymousLongitudeInHawaii - 0.01;
    double endPointAltitudeMeters = 10000.0;
    double radiusMeters = 20000.0;
    cylinder_ = &panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createCylinder(
        startPointLatitudeRadians, startPointLongitudeRadians, startPointAltitudeMeters, endPointLatitudeRadians,
        endPointLongitudeRadians, endPointAltitudeMeters, radiusMeters));
    QColor color(Qt::green);
    color.setAlpha(123);
    cylinder_->setColor(color);
}

void ShapesPlugin::createDisk()
{
    const double latitudeRadians = anonymousLatitudeInHawaii - 0.005;
    const double longitudeRadians = anonymousLongitudeInHawaii - 0.005;
    const double altitudeInMeters = 15000.0;
    const double radiusInMeters = 20000.0;

    LmCdl::I_PanterraShape& disk = panterraApi_->shapeApi().createShape(
        panterraApi_->polygonApi().createDisk(latitudeRadians, longitudeRadians, altitudeInMeters, radiusInMeters));
    disk.setColor(QColor(0, 255, 255, 123));
}

void ShapesPlugin::createArc()
{
    const double latitudeRadians = anonymousLatitudeInHawaii + 0.01;
    const double longitudeRadians = anonymousLongitudeInHawaii - 0.005;
    const double altitudeInMeters = 15000.0;
    const double radiusInMeters = 15000.0;
    const double startAngle = 0.15;
    const double endAngle = 1.05;

    LmCdl::I_PanterraShape& arc = panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createArc(
        latitudeRadians, longitudeRadians, altitudeInMeters, radiusInMeters, startAngle, endAngle));
    arc.setColor(QColor(128, 0, 200, 200));
}

void ShapesPlugin::createRectangularPrism()
{
    double startPointLatitudeRadians = anonymousLatitudeInHawaii - 0.02;
    double startPointLongitudeRadians = anonymousLongitudeInHawaii - 0.01;
    double startPointAltitudeMeters = 40000.0;
    double endPointLatitudeRadians = anonymousLatitudeInHawaii - 0.02;
    double endPointLongitudeRadians = anonymousLongitudeInHawaii - 0.015;
    double endPointAltitudeMeters = 10000.0;
    double widthMeters = 20000.0;
    double heightMeters = 20000.0;
    LmCdl::I_PanterraShape& prism
        = panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createRectangularPrism(
            startPointLatitudeRadians, startPointLongitudeRadians, startPointAltitudeMeters, endPointLatitudeRadians,
            endPointLongitudeRadians, endPointAltitudeMeters, widthMeters, heightMeters));
    QColor color(Qt::red);
    color.setAlpha(200);
    prism.setColor(color);
}

void ShapesPlugin::createVerticalRectangularPrismHighToLow()
{
    double startPointLatitudeRadians = anonymousLatitudeInHawaii - 0.01;
    double startPointLongitudeRadians = anonymousLongitudeInHawaii;
    double startPointAltitudeMeters = 40000.0;
    double endPointLatitudeRadians = anonymousLatitudeInHawaii - 0.01;
    double endPointLongitudeRadians = anonymousLongitudeInHawaii;
    double endPointAltitudeMeters = -1000;
    double widthMeters = 20000.0;
    double heightMeters = 20000.0;
    LmCdl::I_PanterraShape& prism
        = panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createRectangularPrism(
            startPointLatitudeRadians, startPointLongitudeRadians, startPointAltitudeMeters, endPointLatitudeRadians,
            endPointLongitudeRadians, endPointAltitudeMeters, widthMeters, heightMeters));
    QColor color(Qt::yellow);
    color.setAlpha(200);
    prism.setColor(color);
}

void ShapesPlugin::createVerticalRectangularPrismLowToHigh()
{
    double startPointLatitudeRadians = anonymousLatitudeInHawaii - 0.015;
    double startPointLongitudeRadians = anonymousLongitudeInHawaii;
    double startPointAltitudeMeters = -1000.0;
    double endPointLatitudeRadians = anonymousLatitudeInHawaii - 0.015;
    double endPointLongitudeRadians = anonymousLongitudeInHawaii;
    double endPointAltitudeMeters = 40000;
    double widthMeters = 20000.0;
    double heightMeters = 20000.0;
    LmCdl::I_PanterraShape& prism
        = panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createRectangularPrism(
            startPointLatitudeRadians, startPointLongitudeRadians, startPointAltitudeMeters, endPointLatitudeRadians,
            endPointLongitudeRadians, endPointAltitudeMeters, widthMeters, heightMeters));
    QColor color(Qt::yellow);
    color.setAlpha(200);
    prism.setColor(color);
}

void ShapesPlugin::createHorizontalRectangularPrismNorthToSouth()
{
    double startPointLatitudeRadians = anonymousLatitudeInHawaii - 0.01;
    double startPointLongitudeRadians = anonymousLongitudeInHawaii - 0.03;
    double startPointAltitudeMeters = 0;
    double endPointLatitudeRadians = anonymousLatitudeInHawaii - 0.03;
    double endPointLongitudeRadians = anonymousLongitudeInHawaii - 0.03;
    double endPointAltitudeMeters = 0;
    double widthMeters = 20000.0;
    double heightMeters = 20000.0;
    LmCdl::I_PanterraShape& prism
        = panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createRectangularPrism(
            startPointLatitudeRadians, startPointLongitudeRadians, startPointAltitudeMeters, endPointLatitudeRadians,
            endPointLongitudeRadians, endPointAltitudeMeters, widthMeters, heightMeters));
    QColor color(Qt::yellow);
    color.setAlpha(200);
    prism.setColor(color);
}

void ShapesPlugin::createHorizontalRectangularPrismSouthToNorth()
{
    double startPointLatitudeRadians = anonymousLatitudeInHawaii - 0.03;
    double startPointLongitudeRadians = anonymousLongitudeInHawaii - 0.04;
    double startPointAltitudeMeters = 0;
    double endPointLatitudeRadians = anonymousLatitudeInHawaii - 0.01;
    double endPointLongitudeRadians = anonymousLongitudeInHawaii - 0.04;
    double endPointAltitudeMeters = 0;
    double widthMeters = 20000.0;
    double heightMeters = 20000.0;
    LmCdl::I_PanterraShape& prism
        = panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createRectangularPrism(
            startPointLatitudeRadians, startPointLongitudeRadians, startPointAltitudeMeters, endPointLatitudeRadians,
            endPointLongitudeRadians, endPointAltitudeMeters, widthMeters, heightMeters));
    QColor color(Qt::yellow);
    color.setAlpha(200);
    prism.setColor(color);
}

void ShapesPlugin::createHorizontalRectangularPrismEastToWest()
{
    double startPointLatitudeRadians = anonymousLatitudeInHawaii - 0.04;
    double startPointLongitudeRadians = anonymousLongitudeInHawaii - 0.05;
    double startPointAltitudeMeters = 0;
    double endPointLatitudeRadians = anonymousLatitudeInHawaii - 0.04;
    double endPointLongitudeRadians = anonymousLongitudeInHawaii - 0.04;
    double endPointAltitudeMeters = 0;
    double widthMeters = 20000.0;
    double heightMeters = 20000.0;
    LmCdl::I_PanterraShape& prism
        = panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createRectangularPrism(
            startPointLatitudeRadians, startPointLongitudeRadians, startPointAltitudeMeters, endPointLatitudeRadians,
            endPointLongitudeRadians, endPointAltitudeMeters, widthMeters, heightMeters));
    QColor color(Qt::yellow);
    color.setAlpha(200);
    prism.setColor(color);
}

void ShapesPlugin::createHorizontalRectangularPrismWestToEast()
{
    double startPointLatitudeRadians = anonymousLatitudeInHawaii - 0.05;
    double startPointLongitudeRadians = anonymousLongitudeInHawaii - 0.04;
    double startPointAltitudeMeters = 0;
    double endPointLatitudeRadians = anonymousLatitudeInHawaii - 0.05;
    double endPointLongitudeRadians = anonymousLongitudeInHawaii - 0.05;
    double endPointAltitudeMeters = 0;
    double widthMeters = 20000.0;
    double heightMeters = 20000.0;
    LmCdl::I_PanterraShape& prism
        = panterraApi_->shapeApi().createShape(panterraApi_->polygonApi().createRectangularPrism(
            startPointLatitudeRadians, startPointLongitudeRadians, startPointAltitudeMeters, endPointLatitudeRadians,
            endPointLongitudeRadians, endPointAltitudeMeters, widthMeters, heightMeters));
    QColor color(Qt::yellow);
    color.setAlpha(200);
    prism.setColor(color);
}

void ShapesPlugin::startTimerToRemoveSphere() { timerToRemoveSphere_.start(TIME_TO_TOGGLE_VISIBILITY); }

void ShapesPlugin::startTimerToCreateSphere() { timerToCreateSphere_.start(TIME_TO_TOGGLE_VISIBILITY); }

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
Q_EXPORT_PLUGIN2(ShapesPlugin, ShapesPlugin);
#endif
