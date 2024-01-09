/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <I_SingleLightModel.h>
#include <QString>

class NullSingleLightModel : public I_SingleLightModel {
public:
    NullSingleLightModel();
    virtual ~NullSingleLightModel();

public:
    bool isCommandedOn() const override;
    bool isReportedOn() const override;
    bool isSupported() const override;
    bool isAvailable() const override;

    StanagLightType lightType() const override;
    QString lightName() const override;

    void setCommandState(bool state) override;

    static I_SingleLightModel& instance();
};
