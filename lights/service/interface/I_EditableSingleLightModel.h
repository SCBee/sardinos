/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QString>

class I_EditableSingleLightModel {
public:
    virtual ~I_EditableSingleLightModel() { }

public:
    virtual void setReportedState(bool state) = 0;
    virtual void setSupported(bool supported) = 0;
    virtual void setAvailability(bool available) = 0;
    virtual void setLightName(const QString& name) = 0;
};
