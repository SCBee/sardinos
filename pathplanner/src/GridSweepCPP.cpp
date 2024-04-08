#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <utility>

#include "GridSweepCPP.h"

#include "Utils.h"

SweepSearcher::SweepSearcher(const int moving_direction,
                             const int sweep_direction,
                             const std::vector<int>& x_inds_goal_y,
                             const std::optional<int> goal_y)
{
    this->moving_direction = moving_direction;
    this->sweep_direction  = sweep_direction;
    update_turning_window();
    this->x_inds_goal_y = x_inds_goal_y;

    this->goal_y = goal_y.value_or(0);
}

std::pair<std::optional<int>, std::optional<int>>
SweepSearcher::move_target_grid(const int c_x_index,
                                const int c_y_index,
                                GridMap& grid_map)
{
    int n_x_index = this->moving_direction + c_x_index;
    int n_y_index = c_y_index;

    // safe grid
    if (!check_occupied(n_x_index, c_y_index, grid_map)) {
        return {n_x_index, n_y_index};
    } else {  // occupied
        auto next_c = find_safe_turning_grid(c_x_index, c_y_index, grid_map);

        if (!next_c.first.has_value() and !next_c.second.has_value()) {
            next_c.first  = -this->moving_direction + c_x_index;
            next_c.second = c_y_index;

            if (check_occupied(next_c.first, next_c.second, grid_map, 1.0)) {
                return {std::nullopt, std::nullopt};
            }
        } else {
            while (
                !check_occupied(next_c.first.value() + this->moving_direction,
                                next_c.second,
                                grid_map))
            {
                *next_c.first += this->moving_direction;
            }
            swap_moving_direction();
        }
        return next_c;
    }
}

bool SweepSearcher::check_occupied(const std::optional<int> c_x_index,
                                   const std::optional<int> c_y_index,
                                   GridMap& grid_map,
                                   double occupied_val)
{
    if (!c_y_index.has_value() or !c_x_index.has_value()) {
        return true;
    }

    return grid_map.check_occupied_from_xy_index(
        c_x_index.value(), c_y_index.value(), occupied_val);
}

std::pair<std::optional<int>, std::optional<int>>
SweepSearcher::find_safe_turning_grid(const int c_x_index,
                                      const int c_y_index,
                                      GridMap& grid_map)
{
    for (auto& [d_x_ind, d_y_ind] : this->turning_window) {
        int next_x_ind = d_x_ind + c_x_index;
        int next_y_ind = d_y_ind + c_y_index;

        if (!check_occupied(next_x_ind, next_y_ind, grid_map)) {
            return {next_x_ind, next_y_ind};
        }
    }

    return {std::nullopt, std::nullopt};
}

bool SweepSearcher::is_search_done(GridMap& grid_map)
{
    for (auto& ix : this->x_inds_goal_y) {
        if (!check_occupied(ix, this->goal_y, grid_map)) {
            return false;
        }
    }
    return true;
}

void SweepSearcher::update_turning_window()
{
    this->turning_window = {{this->moving_direction, 0},
                            {this->moving_direction, this->sweep_direction},
                            {0, this->sweep_direction},
                            {-this->moving_direction, this->sweep_direction}};
}

void SweepSearcher::swap_moving_direction()
{
    this->moving_direction *= -1;
    update_turning_window();
}

std::pair<std::optional<int>, std::optional<int>>
SweepSearcher::search_start_grid(GridMap& grid_map)
{
    std::vector<int> x_inds;
    std::optional<int> y_ind = 0;

    if (this->sweep_direction == Direction::SweepDirection::DOWN) {
        std::tie(x_inds, y_ind) =
            search_free_grid_index_at_edge_y(grid_map, true);
    } else if (this->sweep_direction == Direction::SweepDirection::UP) {
        std::tie(x_inds, y_ind) =
            search_free_grid_index_at_edge_y(grid_map, false);
    }

    if (this->moving_direction == Direction::MovingDirection::RIGHT) {
        return {*std::min_element(x_inds.begin(), x_inds.end()), y_ind};
    } else if (this->moving_direction == Direction::MovingDirection::LEFT) {
        return {*std::max_element(x_inds.begin(), x_inds.end()), y_ind};
    }
    return {std::nullopt, std::nullopt};
}

std::pair<std::pair<double, double>, std::pair<double, double>>
find_sweep_direction_and_start_position(const std::vector<double>& ox,
                                        const std::vector<double>& oy)
{
    double max_dist = 0.0;
    std::pair<double, double> vec {0.0, 0.0};
    std::pair<double, double> sweep_start_pos {0.0, 0.0};

    for (int i = 0; i < ox.size() - 1; ++i) {
        double dx = ox[i + 1] - ox[i];
        double dy = oy[i + 1] - oy[i];
        double d  = std::sqrt((dx * dx) + (dy * dy));

        if (d > max_dist) {
            max_dist        = d;
            vec             = {dx, dy};
            sweep_start_pos = {ox[i], oy[i]};
        }
    }

    return {vec, sweep_start_pos};
}

std::pair<std::vector<double>, std::vector<double>> convert_grid_coordinate(
    const std::vector<double>& ox,
    const std::vector<double>& oy,
    const std::pair<double, double>& sweep_vec,
    const std::pair<double, double>& sweep_start_position)
{
    // Translate points based on the sweep start position
    std::vector<double> tx(ox.size()), ty(oy.size());
    std::transform(ox.begin(),
                   ox.end(),
                   tx.begin(),
                   [&](double ix) { return ix - sweep_start_position.first; });
    std::transform(oy.begin(),
                   oy.end(),
                   ty.begin(),
                   [&](double iy) { return iy - sweep_start_position.second; });

    // Calculate the angle for the rotation matrix
    double th = std::atan2(sweep_vec.second, sweep_vec.first);

    // Get the rotation matrix
    auto rotation_matrix = utils::rot_mat_2d(th);

    // Apply the rotation matrix
    std::vector<double> converted_x(ox.size()), converted_y(oy.size());
    for (size_t i = 0; i < ox.size(); ++i) {
        converted_x[i] =
            tx[i] * rotation_matrix[0][0] + ty[i] * rotation_matrix[1][0];
        converted_y[i] =
            tx[i] * rotation_matrix[0][1] + ty[i] * rotation_matrix[1][1];
    }

    return {converted_x, converted_y};
}

std::pair<std::vector<double>, std::vector<double>> convert_global_coordinate(
    const std::vector<double>& x,
    const std::vector<double>& y,
    const std::pair<double, double>& sweep_vec,
    const std::pair<double, double>& sweep_start_position)
{
    // Calculate the angle for the rotation matrix
    double th = std::atan2(sweep_vec.second, sweep_vec.first);

    // Get the rotation matrix for the negative angle
    auto rotation_matrix = utils::rot_mat_2d(-th);

    // Apply the rotation matrix
    std::vector<double> rotated_x(x.size()), rotated_y(y.size());
    for (size_t i = 0; i < x.size(); ++i) {
        rotated_x[i] =
            x[i] * rotation_matrix[0][0] + y[i] * rotation_matrix[1][0];
        rotated_y[i] =
            x[i] * rotation_matrix[0][1] + y[i] * rotation_matrix[1][1];
    }

    // Translate points based on the sweep start position
    std::vector<double> rx(rotated_x.size()), ry(rotated_y.size());
    std::transform(rotated_x.begin(),
                   rotated_x.end(),
                   rx.begin(),
                   [&](double ix) { return ix + sweep_start_position.first; });
    std::transform(rotated_y.begin(),
                   rotated_y.end(),
                   ry.begin(),
                   [&](double iy) { return iy + sweep_start_position.second; });

    return {rx, ry};
}

std::pair<std::vector<int>, std::optional<int>>
search_free_grid_index_at_edge_y(GridMap& grid_map, bool from_upper)
{
    std::vector<int> x_range;
    std::vector<int> y_range;
    if (from_upper) {
        for (int i = grid_map.height - 1; i >= 0; --i) {
            x_range.push_back(i);
        }
        for (int i = grid_map.width - 1; i >= 0; --i) {
            y_range.push_back(i);
        }
    } else {
        for (int i = 0; i < grid_map.height; ++i) {
            x_range.push_back(i);
        }
        for (int i = 0; i < grid_map.width; ++i) {
            y_range.push_back(i);
        }
    }

    std::optional<int> y_index = std::nullopt;
    std::vector<int> x_indexes;

    for (auto& iy : x_range) {
        for (auto& ix : y_range) {
            if (!SweepSearcher::check_occupied(ix, iy, grid_map)) {
                y_index = iy;
                x_indexes.push_back(ix);
            }
        }
        if (!y_index.has_value()) {
            break;
        }
    }

    return {x_indexes, y_index};
}

std::tuple<GridMap, std::vector<int>, std::optional<int>> setup_grid_map(
    std::vector<double>& ox,
    std::vector<double>& oy,
    double resolution,
    int sweep_direction,
    int offset_grid)
{
    int width =
        static_cast<int>(std::ceil((*std::max_element(ox.begin(), ox.end())
                                    - *std::min_element(ox.begin(), ox.end()))
                                   / resolution))
        + offset_grid;
    int height =
        static_cast<int>(std::ceil((*std::max_element(oy.begin(), oy.end())
                                    - *std::min_element(oy.begin(), oy.end()))
                                   / resolution))
        + offset_grid;
    double center_x = (*std::max_element(ox.begin(), ox.end())
                       + *std::min_element(ox.begin(), ox.end()))
        / 2.0;
    double center_y = (*std::max_element(oy.begin(), oy.end())
                       + *std::min_element(oy.begin(), oy.end()))
        / 2.0;

    GridMap grid_map(width, height, resolution, center_x, center_y);
    grid_map.print_grid_map_info();
    grid_map.set_value_from_polygon(ox, oy, 1.0, false);
    grid_map.expand_grid();

    std::vector<int> x_inds_goal_y;
    std::optional<int> goal_y = std::nullopt;

    if (sweep_direction == Direction::SweepDirection::UP) {
        std::tie(x_inds_goal_y, goal_y) =
            search_free_grid_index_at_edge_y(grid_map, true);

    } else if (sweep_direction == Direction::SweepDirection::DOWN) {
        std::tie(x_inds_goal_y, goal_y) =
            search_free_grid_index_at_edge_y(grid_map, false);
    }

    return std::make_tuple(grid_map, x_inds_goal_y, goal_y);
}

std::pair<std::vector<double>, std::vector<double>> sweepPathSearch(
    SweepSearcher& sweepSearcher, GridMap& gridMap)
{
    auto cIndex = sweepSearcher.search_start_grid(gridMap);

    if (!cIndex.first.has_value() or !cIndex.second.has_value()
        or !gridMap.set_value_from_xy_index(cIndex.first, cIndex.second, 0.5))
    {
        return {{}, {}};
    }

    auto [x, y] = gridMap.calc_grid_central_xy_position_from_xy_index(
        cIndex.first.value(), cIndex.second.value());
    std::vector<double> px = {x}, py = {y};

    while (true) {
        cIndex = sweepSearcher.move_target_grid(
            cIndex.first.value(), cIndex.second.value(), gridMap);

        if (sweepSearcher.is_search_done(gridMap)
            || (!cIndex.first.has_value() or !cIndex.second.has_value()))
        {
            break;
        }

        auto tmp = gridMap.calc_grid_central_xy_position_from_xy_index(
            cIndex.first.value(), cIndex.second.value());
        px.push_back(tmp.first);
        py.push_back(tmp.second);

        gridMap.set_value_from_xy_index(
            cIndex.first.value(), cIndex.second.value(), 0.5);
    }

    return {px, py};
}

std::pair<std::vector<double>, std::vector<double>> planning(
    const std::vector<double>& ox,
    const std::vector<double>& oy,
    double resolution,
    int movingDirection,
    int sweepingDirection)
{
    auto [sweepVec, sweepStartPosition] =
        find_sweep_direction_and_start_position(ox, oy);

    std::cout << "1\n";

    auto [rox, roy] =
        convert_grid_coordinate(ox, oy, sweepVec, sweepStartPosition);
    std::cout << "2\n";

    auto planning_objects =
        setup_grid_map(rox, roy, resolution, sweepingDirection);
    std::cout << "3\n";

    SweepSearcher sweepSearcher(movingDirection,
                                sweepingDirection,
                                std::get<1>(planning_objects),
                                std::get<2>(planning_objects));
    std::cout << "4\n";

    auto [px, py] =
        sweepPathSearch(sweepSearcher, std::get<0>(planning_objects));
    std::cout << "5\n";

    auto [rx, ry] =
        convert_global_coordinate(px, py, sweepVec, sweepStartPosition);
    std::cout << "6\n";

    return {rx, ry};
}
