/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <I_LightsStateModel.h>
#include <QHash>
#include <QObject>
#include <QString>
#include <StanagLightType.h>

class SingleLightModel;

class LightsStateModel : public QObject, public I_LightsStateModel {
    Q_OBJECT

public:
    LightsStateModel();
    virtual ~LightsStateModel() override;

    I_SingleLightModel& lightModel(StanagLightType type) override;
    I_EditableSingleLightModel& editableLightModel(StanagLightType type) override;
    void setIsCucsInControlOfVehicle(bool inControl) override;

signals:
    void anyLightCommandStateChanged() override;

private:
    void createSingleLightModelSupportedByDefault(StanagLightType type, const QString& name);
    void createSingleLightModelNotSupportedByDefault(StanagLightType type);

private:
    QHash<StanagLightType, SingleLightModel*> lights_;
    bool cucsInControlOfVehicle_;
};
