/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QString>
#include <StanagLightType.h>

class I_SingleLightModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isCommandedOn READ isCommandedOn WRITE setCommandState NOTIFY commandChanged);
    Q_PROPERTY(bool isReportedOn READ isReportedOn NOTIFY reportChanged);
    Q_PROPERTY(bool isSupported READ isSupported NOTIFY supportChanged);
    Q_PROPERTY(bool isAvailable READ isAvailable NOTIFY availabilityChanged);
    Q_PROPERTY(QString lightName READ lightName NOTIFY lightNameChanged);

public:
    virtual ~I_SingleLightModel() { }

public:
    virtual bool isCommandedOn() const = 0;
    virtual bool isReportedOn() const = 0;
    virtual bool isSupported() const = 0;
    virtual bool isAvailable() const = 0;

    virtual StanagLightType lightType() const = 0;
    virtual QString lightName() const = 0;

public slots:
    virtual void setCommandState(bool state) = 0;

signals:
    void commandChanged(bool isCommandedOn);
    void reportChanged(bool isReportedOn);
    void supportChanged(bool supported);
    void availabilityChanged(bool availability);
    void lightNameChanged(QString lightName);
};
