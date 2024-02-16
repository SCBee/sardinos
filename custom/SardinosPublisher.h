//
// Created by dev on 2/3/2024.
//

#ifndef VCSI_SARDINOS_SARDINOSPUBLISHER_H
#define VCSI_SARDINOS_SARDINOSPUBLISHER_H

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <thread>

#include <MathExt.h>
#include <MissionPlanningContentCreator.h>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <windows.h>

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

Mission::MissionItem make_mission_item(
    double latitude_deg,
    double longitude_deg,
    float relative_altitude_m,
    float speed_m_s,
    bool is_fly_through,
    float gimbal_pitch_deg,
    float gimbal_yaw_deg,
    Mission::MissionItem::CameraAction camera_action)
{
    Mission::MissionItem new_item {};
    new_item.latitude_deg        = latitude_deg;
    new_item.longitude_deg       = longitude_deg;
    new_item.relative_altitude_m = relative_altitude_m;
    new_item.speed_m_s           = speed_m_s;
    new_item.is_fly_through      = is_fly_through;
    new_item.gimbal_pitch_deg    = gimbal_pitch_deg;
    new_item.gimbal_yaw_deg      = gimbal_yaw_deg;
    new_item.camera_action       = camera_action;
    return new_item;
}

namespace sardinos
{
    void setColor(const std::string& textColor = "default",
                  const std::string& bgColor   = "black")
    {
        // Map of color strings to their corresponding Windows Console color
        // codes
        std::map<std::string, int> colors {{"black", 0},
                                           {"blue", 1},
                                           {"green", 2},
                                           {"cyan", 3},
                                           {"red", 4},
                                           {"magenta", 5},
                                           {"brown", 6},
                                           {"default", 7},
                                           {"darkgray", 8},
                                           {"lightblue", 9},
                                           {"lightgreen", 10},
                                           {"lightcyan", 11},
                                           {"lightred", 12},
                                           {"lightmagenta", 13},
                                           {"yellow", 14},
                                           {"white", 15}};

        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        // Find the color codes from the map, use light gray (7) if not found
        int textCode =
            colors.find(textColor) != colors.end() ? colors[textColor] : 7;
        int bgCode = colors.find(bgColor) != colors.end() ? colors[bgColor] : 0;
        SetConsoleTextAttribute(consoleHandle,
                                (WORD)((bgCode << 4) | textCode));
    }

    void executeMissionVTOL(
        const std::vector<std::pair<float, float>>& waypoints,
        volatile double& lat_,
        volatile double& lon_,
        volatile double& alt_,
        volatile double& altAbs_,
        volatile double& head_,
        volatile double& speed_,
        volatile double& yaw_,
        volatile double& batt_)
    {
        auto connectStr = "udp://:14550";
        Mavsdk mavsdk {
            Mavsdk::Configuration {Mavsdk::ComponentType::GroundStation}};
        ConnectionResult connection_result =
            mavsdk.add_any_connection(connectStr);

        // wait while we try to establish our connection (heartbeat needs to be
        // recorded)
        setColor("red");
        while (connection_result != ConnectionResult::Success) {
            std::cerr << "Connection failed: " << connection_result << '\n';
            sleep_for(seconds(3));
            connection_result = mavsdk.add_any_connection(connectStr);
        }

        // wait while we connect to the system
        setColor("red");
        auto system = mavsdk.first_autopilot(3.0);
        while (!system) {
            std::cerr << "Timed out waiting for system...\n";
            sleep_for(seconds(3));
        }

        // Instantiate plugins.
        auto telemetry = Telemetry {system.value()};
        auto action    = Action {system.value()};

        // We want to listen to the altitude of the drone at 1 Hz.
        setColor("red");
        Telemetry::Result set_rate_result = telemetry.set_rate_position(1.0);
        while (set_rate_result != Telemetry::Result::Success) {
            std::cerr << "Setting rate failed: " << set_rate_result << '\n';
            sleep_for(seconds(3));
            set_rate_result = telemetry.set_rate_position(1.0);
        }

        telemetry.subscribe_heading([&head_](const Telemetry::Heading& headTel)
                                    { head_ = headTel.heading_deg; });

        telemetry.subscribe_position(
            [&lat_, &lon_, &alt_, &altAbs_](const Telemetry::Position& position)
            {
                lat_    = position.latitude_deg;
                lon_    = position.longitude_deg;
                alt_    = position.relative_altitude_m;
                altAbs_ = position.absolute_altitude_m;
            });

        telemetry.subscribe_velocity_ned(
            [&speed_](const Telemetry::VelocityNed& vel)
            {
                double vabs = std::sqrt(std::pow(vel.north_m_s, 2)
                                        + std::pow(vel.east_m_s, 2)
                                        + std::pow(vel.down_m_s, 2));
                speed_      = vabs;
            });

        telemetry.subscribe_attitude_euler(
            [&yaw_](const Telemetry::EulerAngle& euler)
            { yaw_ = euler.yaw_deg; });

        telemetry.subscribe_battery(
            [&batt_](const Telemetry::Battery& battery)
            { batt_ = battery.remaining_percent / 100.0; });

        // Wait until we are ready to arm.
        setColor("cyan");
        while (!telemetry.health_all_ok()) {
            std::cout << "Waiting for vehicle to be ready to arm...\n";
            sleep_for(seconds(2));
        }

        // Arm vehicle
        setColor("red");
        Action::Result arm_result = action.arm();
        while (arm_result != Action::Result::Success) {
            std::cerr << "Arming failed: " << arm_result << '\n';
            sleep_for(seconds(2));
            arm_result = action.arm();
        }

        // take off altitude
        double height = 150.0;
        action.set_takeoff_altitude(height);

        // Take off
        setColor("red");
        Action::Result takeoff_result = action.takeoff();
        while (takeoff_result != Action::Result::Success) {
            std::cerr << "Takeoff failed: " << takeoff_result << '\n';
            sleep_for(seconds(2));
            takeoff_result = action.takeoff();
        }

        // Wait while it takes off.
        setColor("yellow");
        std::cout << "Waiting to reach takeoff altitude...\n";
        while (alt_ <= height - 2) {
            sleep_for(seconds(1));
        }

        // transition to fixed wing
        setColor("red");
        Action::Result fw_result = action.transition_to_fixedwing();
        while (fw_result != Action::Result::Success) {
            std::cerr << "Transition to fixed wing failed: " << fw_result
                      << '\n';
            sleep_for(seconds(1));
            fw_result = action.transition_to_fixedwing();
        }

        // Let it transition and start loitering.
        setColor("yellow");
        std::cout << "loitering...\n";
        sleep_for(seconds(3));

        // go through path
        setColor("red");
        bool outOfPath = false;
        for (auto& [longitude, latitude] : waypoints) {
            std::cout << "Sending it to location: (" << latitude << ", "
                      << longitude << ")" << std::endl;

            Action::Result goto_result =
                action.goto_location(latitude, longitude, NAN, NAN);

            while (goto_result != Action::Result::Success) {
                std::cerr << "Goto command failed: " << goto_result << " ["
                          << latitude << ", " << longitude << "]\n";
                sleep_for(seconds(3));
                goto_result =
                    action.goto_location(latitude, longitude, NAN, NAN);
            }
            while (std::abs(lat_ - latitude) > 0.001
                   || std::abs(lon_ - longitude) > 0.001)
            {
                double distance = sardinos::MathExt::getDistance(
                    lat_, lon_, latitude, longitude);

                if (distance > 5.0f) {
                    if (!outOfPath) {
                        sardinos::setColor("red");
                        std::cout << "Drone is " << distance
                                  << " meters off the path!" << std::endl;
                        outOfPath = true;
                    }
                } else {
                    if (outOfPath) {
                        sardinos::setColor("green");
                        std::cout << "Drone is back on track" << std::endl;
                        outOfPath = false;
                    }
                }
                sardinos::setColor("default");
                sleep_for(seconds(1));
            }
        }

        // RTL

        // Let's stop before reaching the goto point and go back to hover.
        setColor("red");
        Action::Result mc_result = action.transition_to_multicopter();
        while (mc_result != Action::Result::Success) {
            std::cerr << "Transition to multi copter failed: " << mc_result
                      << '\n';
            sleep_for(seconds(2));
            mc_result = action.transition_to_multicopter();
        }

        // Wait for the transition to be carried out.
        sleep_for(seconds(5));

        // Now just land here.
        setColor("red");
        const Action::Result land_result = action.land();
        if (land_result != Action::Result::Success) {
            std::cerr << "Land failed: " << land_result << '\n';
            return;
        }

        // Wait until disarmed.
        setColor("yellow");
        while (telemetry.armed()) {
            std::cout << "Waiting for vehicle to land and disarm\n";
            sleep_for(seconds(2));
        }

        std::cout << "Disarmed, exiting.\n";
    }
}  // namespace sardinos

#endif  // VCSI_SARDINOS_SARDINOSPUBLISHER_H
