/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <I_EditableSingleLightModel.h>
#include <QString>

class NullEditableSingleLightModel : public I_EditableSingleLightModel {
public:
    NullEditableSingleLightModel();
    virtual ~NullEditableSingleLightModel();

    void setReportedState(bool reportedState) override;
    void setAvailability(bool availability) override;
    void setSupported(bool supported) override;
    void setLightName(const QString& name) override;

    static I_EditableSingleLightModel& instance();
};
