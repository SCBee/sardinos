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

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

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
    new_item.latitude_deg = latitude_deg;
    new_item.longitude_deg = longitude_deg;
    new_item.relative_altitude_m = relative_altitude_m;
    new_item.speed_m_s = speed_m_s;
    new_item.is_fly_through = is_fly_through;
    new_item.gimbal_pitch_deg = gimbal_pitch_deg;
    new_item.gimbal_yaw_deg = gimbal_yaw_deg;
    new_item.camera_action = camera_action;
    return new_item;
}

namespace sardinos
{
void executeMission(std::vector<std::pair<float, float>>& waypoints)
{
    auto connectStr = "udp://:14550";
    Mavsdk mavsdk {
        Mavsdk::Configuration {Mavsdk::ComponentType::GroundStation}};
    ConnectionResult connection_result = mavsdk.add_any_connection(connectStr);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return;
    } else {
        std::cout << "Connection successful\n";
    }

    auto system = mavsdk.first_autopilot(3.0);
    if (!system) {
        std::cerr << "Timed out waiting for system\n";
        return;
    } else {
        std::cout << "System initialization successful\n";
    }

    auto action = Action {system.value()};
    auto mission = Mission {system.value()};
    auto telemetry = Telemetry {system.value()};

    while (!telemetry.health_all_ok()) {
        std::cout << "Waiting for system to be ready\n";
        sleep_for(seconds(1));
    }

    // Set position update rate to 1 Hz.
    const Telemetry::Result set_rate_result = telemetry.set_rate_position(0.5);
    if (set_rate_result != Telemetry::Result::Success) {
        // handle rate-setting failure (in this case print error)
        std::cout << "Setting rate failed:" << set_rate_result << '\n';
    }
    const Telemetry::Result set_odometry_rate_result =
        telemetry.set_rate_odometry(0.5);
    if (set_odometry_rate_result != Telemetry::Result::Success) {
        std::cerr << "Setting odometry rate failed:" << set_rate_result << "\n";
    }

    std::cout << "System ready\n";
    std::cout << "Creating and uploading mission\n";

    std::vector<Mission::MissionItem> mission_items;

    const float defaultAltitude = 10.0f;
    const float defaultSpeed = 25.0f;
    const float defaultGimbalPitch = 0.0f;
    const float defaultGimbalYaw = 0.0f;
    const auto defaultCamera = Mission::MissionItem::CameraAction::None;

    for (auto& [longitude, latitude] : waypoints) {
        mission_items.push_back(make_mission_item(latitude,
                                                  longitude,
                                                  defaultAltitude,
                                                  defaultSpeed,
                                                  true,
                                                  defaultGimbalPitch,
                                                  defaultGimbalYaw,
                                                  defaultCamera));
    }

    std::cout << "Uploading mission...\n";
    Mission::MissionPlan mission_plan {};
    mission_plan.mission_items = mission_items;
    const Mission::Result upload_result = mission.upload_mission(mission_plan);

    if (upload_result != Mission::Result::Success) {
        std::cerr << "Mission upload failed: " << upload_result
                  << ", exiting.\n";
        return;
    }

    std::cout << "Arming...\n";
    const Action::Result arm_result = action.arm();
    if (arm_result != Action::Result::Success) {
        std::cerr << "Arming failed: " << arm_result << '\n';
        return;
    }
    std::cout << "Armed.\n";

    telemetry.subscribe_position(
        [](const Telemetry::Position& position)
        {
            std::cout << "Altitude: " << position.relative_altitude_m << " m"
                      << std::endl
                      << "Latitude: " << position.latitude_deg << std::endl
                      << "Longitude: " << position.longitude_deg << std::endl
                      << std::endl;
        });

    telemetry.subscribe_odometry(
        [](const Telemetry::Odometry& odom)
        {
            std::cout << "[time] " << odom.time_usec << " ms" << std::endl
                      << "[x-vel] " << odom.velocity_body.x_m_s << " m/s"
                      << std::endl
                      << "[y-vel] " << odom.velocity_body.y_m_s << " m/s"
                      << std::endl
                      << "[z-vel] " << odom.velocity_body.z_m_s << " m/s"
                      << std::endl
                      << std::endl;
        });

    std::atomic<bool> want_to_pause {false};
    // Before starting the mission, we want to be sure to subscribe to the
    // mission progress.
    mission.subscribe_mission_progress(
        [&want_to_pause](Mission::MissionProgress mission_progress)
        {
            std::cout << "Mission status update: " << mission_progress.current
                      << " / " << mission_progress.total << '\n';

            if (mission_progress.current >= 2) {
                // We can only set a flag here. If we do more request inside the
                // callback, we risk blocking the system.
                want_to_pause = true;
            }
        });

    Mission::Result start_mission_result = mission.start_mission();
    if (start_mission_result != Mission::Result::Success) {
        std::cerr << "Starting mission failed: " << start_mission_result
                  << '\n';
        return;
    }

    while (!want_to_pause) {
        sleep_for(seconds(1));
    }

    std::cout << "Pausing mission...\n";
    const Mission::Result pause_mission_result = mission.pause_mission();

    if (pause_mission_result != Mission::Result::Success) {
        std::cerr << "Failed to pause mission:" << pause_mission_result << '\n';
    }
    std::cout << "Mission paused.\n";

    // Pause for 5 seconds.
    sleep_for(seconds(5));

    // Then continue.
    Mission::Result start_mission_again_result = mission.start_mission();
    if (start_mission_again_result != Mission::Result::Success) {
        std::cerr << "Starting mission again failed: "
                  << start_mission_again_result << '\n';
        return;
    }

    while (!mission.is_mission_finished().second) {
        sleep_for(seconds(1));
    }

    // We are done, and can do RTL to go home.
    std::cout << "Commanding RTL...\n";
    const Action::Result rtl_result = action.return_to_launch();
    if (rtl_result != Action::Result::Success) {
        std::cout << "Failed to command RTL: " << rtl_result << '\n';
        return;
    }
    std::cout << "Commanded RTL.\n";

    // We need to wait a bit, otherwise the armed state might not be correct
    // yet.
    sleep_for(seconds(2));

    while (telemetry.armed()) {
        // Wait until we're done.
        sleep_for(seconds(1));
    }
    std::cout << "Disarmed, exiting.\n";
}
}  // namespace sardinos

#endif  // VCSI_SARDINOS_SARDINOSPUBLISHER_H