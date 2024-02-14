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
    void setColor(const std::string& textColor,
                  const std::string& bgColor = "black")
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

    void executeMissionVTOL(std::vector<std::pair<float, float>>& waypoints,
                            volatile double& lat_,
                            volatile double& lon_,
                            volatile double& alt_,
                            volatile double& head_)
    {
        auto connectStr = "udp://:14550";
        Mavsdk mavsdk {
            Mavsdk::Configuration {Mavsdk::ComponentType::GroundStation}};
        ConnectionResult connection_result =
            mavsdk.add_any_connection(connectStr);

        if (connection_result != ConnectionResult::Success) {
            std::cerr << "Connection failed: " << connection_result << '\n';
            return;
        }

        auto system = mavsdk.first_autopilot(3.0);
        if (!system) {
            std::cerr << "Timed out waiting for system\n";
            return;
        }

        // Instantiate plugins.
        auto telemetry = Telemetry {system.value()};
        auto action    = Action {system.value()};

        // We want to listen to the altitude of the drone at 1 Hz.
        const Telemetry::Result set_rate_result =
            telemetry.set_rate_position(1.0);
        if (set_rate_result != Telemetry::Result::Success) {
            std::cerr << "Setting rate failed: " << set_rate_result << '\n';
            return;
        }

        telemetry.subscribe_heading(
            [&head_](const Telemetry::Heading& headTel)
            {
                //            std::cout << "Heading: " << headTel.heading_deg <<
                //            " deg"
                //                      << std::endl;
                head_ = headTel.heading_deg;
            });

        telemetry.subscribe_position(
            [&lat_, &lon_, &alt_](const Telemetry::Position& position)
            {
                //            std::cout << "Altitude: " <<
                //            position.relative_altitude_m << " m"
                //                      << std::endl
                //                      << "Latitude: " << position.latitude_deg
                //                      << std::endl
                //                      << "Longitude: " <<
                //                      position.longitude_deg
                //                      << std::endl
                //                      << std::endl;
                lat_ = position.latitude_deg;
                lon_ = position.longitude_deg;
                alt_ = position.relative_altitude_m;
            });

        // Wait until we are ready to arm.
        while (!telemetry.health_all_ok()) {
            std::cout << "Waiting for vehicle to be ready to arm...\n";
            sleep_for(seconds(1));
        }

        // Arm vehicle
        std::cout << "Arming.\n";
        const Action::Result arm_result = action.arm();

        if (arm_result != Action::Result::Success) {
            std::cerr << "Arming failed: " << arm_result << '\n';
            return;
        }

        action.set_takeoff_altitude(200.0f);
        sleep_for(seconds(2));

        // Take off
        std::cout << "Taking off.\n";
        const Action::Result takeoff_result = action.takeoff();
        if (takeoff_result != Action::Result::Success) {
            std::cerr << "Takeoff failed:n" << takeoff_result << '\n';
            return;
        }

        // Wait while it takes off.
        while (alt_ <= 199.0f) {
            sleep_for(seconds(2));
        }
        sleep_for(seconds(3));

        std::cout << "Transition to fixedwing.\n";
        const Action::Result fw_result = action.transition_to_fixedwing();

        if (fw_result != Action::Result::Success) {
            std::cerr << "Transition to fixed wing failed: " << fw_result
                      << '\n';
            return;
        }

        // Let it transition and start loitering.
        sleep_for(seconds(10));

        bool outOfPath = false;

        for (auto& [longitude, latitude] : waypoints) {
            std::cout << "Sending it to location: (" << latitude << ", "
                      << longitude << ")" << std::endl;

            const Action::Result goto_result =
                action.goto_location(latitude, longitude, NAN, NAN);

            if (goto_result != Action::Result::Success) {
                std::cerr << "Goto command failed: " << goto_result << '\n';
                return;
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

        // Let's stop before reaching the goto point and go back to hover.
        std::cout << "Transition back to multicopter...\n";
        const Action::Result mc_result = action.transition_to_multicopter();
        if (mc_result != Action::Result::Success) {
            std::cerr << "Transition to multi copter failed: " << mc_result
                      << '\n';
            return;
        }

        // Wait for the transition to be carried out.
        sleep_for(seconds(5));

        // Now just land here.
        std::cout << "Landing...\n";
        const Action::Result land_result = action.land();
        if (land_result != Action::Result::Success) {
            std::cerr << "Land failed: " << land_result << '\n';
            return;
        }

        // Wait until disarmed.
        while (telemetry.armed()) {
            std::cout << "Waiting for vehicle to land and disarm\n.";
            sleep_for(seconds(1));
        }

        std::cout << "Disarmed, exiting.\n";
    }
}  // namespace sardinos

#endif  // VCSI_SARDINOS_SARDINOSPUBLISHER_H
