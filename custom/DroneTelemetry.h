//
// Created by smut on 2/25/2024.
//

#ifndef VCSI_SARDINOS_DRONETELEMETRY_H
#define VCSI_SARDINOS_DRONETELEMETRY_H

#include <QObject>
#include <mutex>

class DroneTelemetry : public QObject
{
    Q_OBJECT

public:
    explicit DroneTelemetry();

    // Setters
    void setLatitude(double latitude);
    void setLongitude(double longitude);
    void setAltitude(double altitude);
    void setAltitudeAbs(double altitudeAbs);
    void setHeading(double heading);
    void setSpeed(double speed);
    void setYaw(double yaw);
    void setBattery(double battery);
    void setConnectionStatus(bool status);

    // Getters
    double latitude() const;
    double longitude() const;
    double altitude() const;
    double altitudeAbs() const;
    double heading() const;
    double speed() const;
    double yaw() const;
    double battery() const;
    bool isConnected() const;

signals:
    void telemetryUpdated();
    void connectionStatusChanged(bool connected);

private:
    mutable std::mutex dataMutex;
    double m_latitude    = 0.0;
    double m_longitude   = 0.0;
    double m_altitude    = 0.0;
    double m_altitudeAbs = 0.0;
    double m_heading     = 0.0;
    double m_speed       = 0.0;
    double m_yaw         = 0.0;
    double m_battery     = 0.0;
    bool m_connected     = false;
};

#endif  // VCSI_SARDINOS_DRONETELEMETRY_H
