/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QTimer>
#include <StanagLightType.h>

class I_LightsStateModel;
class SingleLightVsmSimulation;

namespace LmCdl {
class I_Vehicle;
}

/* This fake simulator just turns on the lights without waiting for a response from the
 * vcsi simulator
 * It's entirely used for demo purposes as the real simulator does not
 * currently have lights support
 */
class LightsVsmSimulation : public QObject {
public:
    Q_OBJECT

public:
    LightsVsmSimulation(LmCdl::I_Vehicle& vehicle, I_LightsStateModel& lightsStateModel);
    virtual ~LightsVsmSimulation();

private:
    void addDemoForLight(StanagLightType type);

private:
    I_LightsStateModel& lightsStateModel_;
    QList<SingleLightVsmSimulation*> singleLightSimulation_;
};
