/**
\file
Copyright (c) 2016 Lockheed Martin Canada Inc. All rights reserved.
*/

#pragma once

#include <LmCdl/I_Plugin.h>
#include <QObject>
#include <QPointer>
#include <QTimer>

namespace LmCdl {
class I_PanterraPublicApi;
class I_PanterraShape;
}

class ShapesPlugin : public QObject, public LmCdl::I_Plugin {
    Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    explicit ShapesPlugin();
    virtual ~ShapesPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;
    LmCdl::PluginCapabilityIdentifier providedApi() const override;
    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject* api) override;
    QObject* getProvidedApi() override;
    bool isFullyInitialized() const override;

private slots:
    void createSphere();
    void removeSphere();

private:
    void createStaticShapes();
    void createCylinder();
    void createDisk();
    void createArc();
    void createRectangularPrism();
    void createVerticalRectangularPrismHighToLow();
    void createVerticalRectangularPrismLowToHigh();
    void createHorizontalRectangularPrismNorthToSouth();
    void createHorizontalRectangularPrismSouthToNorth();
    void createHorizontalRectangularPrismEastToWest();
    void createHorizontalRectangularPrismWestToEast();

    void startTimerToRemoveSphere();
    void startTimerToCreateSphere();

private:
    LmCdl::I_PanterraPublicApi* panterraApi_;

    LmCdl::I_PanterraShape* sphere_;
    LmCdl::I_PanterraShape* cylinder_;
    QTimer timerToRemoveSphere_;
    QTimer timerToCreateSphere_;
};
