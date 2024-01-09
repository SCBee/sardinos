/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <I_LightsStateModel.h>
#include <QObject>

class LightsPresenter : public QObject {
public:
    Q_OBJECT

public:
    LightsPresenter(I_LightsStateModel& model, QObject* parent = nullptr);

    Q_INVOKABLE I_SingleLightModel* navLight();
    Q_INVOKABLE I_SingleLightModel* navIrLight();
    Q_INVOKABLE I_SingleLightModel* strobeLight();
    Q_INVOKABLE I_SingleLightModel* strobeIrLight();
    Q_INVOKABLE I_SingleLightModel* nvdLight();
    Q_INVOKABLE I_SingleLightModel* landingLight();
    Q_INVOKABLE I_SingleLightModel* landingIrLight();
    Q_INVOKABLE I_SingleLightModel* reservedLight1();
    Q_INVOKABLE I_SingleLightModel* vehicleSpecificLight1();
    Q_INVOKABLE I_SingleLightModel* vehicleSpecificLight2();
    Q_INVOKABLE I_SingleLightModel* vehicleSpecificLight3();
    Q_INVOKABLE I_SingleLightModel* vehicleSpecificLight4();
    Q_INVOKABLE I_SingleLightModel* vehicleSpecificLight5();
    Q_INVOKABLE I_SingleLightModel* vehicleSpecificLight6();
    Q_INVOKABLE I_SingleLightModel* vehicleSpecificLight7();
    Q_INVOKABLE I_SingleLightModel* vehicleSpecificLight8();

private:
    I_LightsStateModel& model_;
};
