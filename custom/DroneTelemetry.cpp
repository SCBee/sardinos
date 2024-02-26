//
// Created by smut on 2/25/2024.
//

#include <iostream>

#include "DroneTelemetry.h"

DroneTelemetry::DroneTelemetry()  = default;

// Setters
void DroneTelemetry::setLatitude(double latitude)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_latitude != latitude) {
        m_latitude = latitude;
        emit telemetryUpdated();
    }
}

void DroneTelemetry::setLongitude(double longitude)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_longitude != longitude) {
        m_longitude = longitude;
        emit telemetryUpdated();
    }
}

void DroneTelemetry::setAltitude(double altitude)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_altitude != altitude) {
        m_altitude = altitude;
        emit telemetryUpdated();
    }
}

void DroneTelemetry::setAltitudeAbs(double altitudeAbs)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_altitudeAbs != altitudeAbs) {
        m_altitudeAbs = altitudeAbs;
        emit telemetryUpdated();
    }
}

void DroneTelemetry::setHeading(double heading)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_heading != heading) {
        m_heading = heading;
        emit telemetryUpdated();
    }
}

void DroneTelemetry::setSpeed(double speed)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_speed != speed) {
        m_speed = speed;
        emit telemetryUpdated();
    }
}

void DroneTelemetry::setYaw(double yaw)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_yaw != yaw) {
        m_yaw = yaw;
        emit telemetryUpdated();
    }
}

void DroneTelemetry::setBattery(double battery)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_battery != battery) {
        m_battery = battery;
        emit telemetryUpdated();
    }
}

void DroneTelemetry::setConnectionStatus(bool status)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if (m_connected != status) {
        m_connected = status;
        emit connectionStatusChanged(status);
    }
}

// Getters
double DroneTelemetry::latitude() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_latitude;
}

double DroneTelemetry::longitude() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_longitude;
}

double DroneTelemetry::altitude() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_altitude;
}

double DroneTelemetry::altitudeAbs() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_altitudeAbs;
}

double DroneTelemetry::heading() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_heading;
}

double DroneTelemetry::speed() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_speed;
}

double DroneTelemetry::yaw() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_yaw;
}

double DroneTelemetry::battery() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_battery;
}

bool DroneTelemetry::isConnected() const
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return m_connected;
}
