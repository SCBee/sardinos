/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QTimer>

class I_SingleLightModel;
class I_EditableSingleLightModel;

class SingleLightVsmSimulation : public QObject {
    Q_OBJECT

public:
    SingleLightVsmSimulation(I_SingleLightModel& light, I_EditableSingleLightModel& editableLight);
    virtual ~SingleLightVsmSimulation();

public slots:
    void syncState();

private:
    I_SingleLightModel& light_;
    I_EditableSingleLightModel& editableLight_;
    QTimer timer_;
};
