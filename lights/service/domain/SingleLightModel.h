/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <I_EditableSingleLightModel.h>
#include <I_SingleLightModel.h>
#include <QString>

class SingleLightModel : public I_SingleLightModel, public I_EditableSingleLightModel {
public:
    SingleLightModel(StanagLightType type);
    virtual ~SingleLightModel();

public:
    bool isCommandedOn() const override;
    bool isReportedOn() const override;
    bool isSupported() const override;
    bool isAvailable() const override;

    StanagLightType lightType() const override;
    QString lightName() const override;

    void setCommandState(bool state) override;
    void setReportedState(bool reportedState) override;
    void setSupported(bool supported) override;
    void setAvailability(bool availability) override;
    void setLightName(const QString& name) override;

private:
    bool isCommandedOn_;
    bool isReportedOn_;
    bool isAvailable_;
    bool isSupported_;

    StanagLightType lightType_;
    QString lightName_;
};
