#ifndef INCLUDE_INCLUDES_GRIDSWEEPCPP_H_
#define INCLUDE_INCLUDES_GRIDSWEEPCPP_H_

#include "GridMap.h"

namespace Direction
{

    namespace SweepDirection
    {
        constexpr int UP   = 1;
        constexpr int DOWN = -1;
    };  // namespace SweepDirection

    namespace MovingDirection
    {
        constexpr int RIGHT = 1;
        constexpr int LEFT  = -1;
    };  // namespace MovingDirection

};  // namespace Direction

class SweepSearcher
{
private:
    int moving_direction;
    int sweep_direction;
    int goal_y;
    std::vector<std::pair<int, int>> turning_window;
    std::vector<int> x_inds_goal_y;

public:
    SweepSearcher(const int moving_direction,
                  const int sweep_direction,
                  const std::vector<int>& x_inds_goal_y,
                  const std::optional<int> goal_y);

    std::pair<std::optional<int>, std::optional<int>> move_target_grid(
        const int c_x_index, const int c_y_index, GridMap& grid_map);

    static bool check_occupied(const std::optional<int> c_x_index,
                               const std::optional<int> c_y_index,
                               GridMap& grid_map,
                               double occupied_val = 0.5);

    std::pair<std::optional<int>, std::optional<int>> find_safe_turning_grid(
        const int c_x_index, const int c_y_index, GridMap& grid_map);

    bool is_search_done(GridMap& grid_map);

    void update_turning_window();

    void swap_moving_direction();

    std::pair<std::optional<int>, std::optional<int>> search_start_grid(
        GridMap& grid_map);
};

std::pair<std::pair<double, double>, std::pair<double, double>>
find_sweep_direction_and_start_position(const std::vector<double>& ox,
                                        const std::vector<double>& oy);

std::pair<std::vector<double>, std::vector<double>> convert_grid_coordinate(
    const std::vector<double>& ox,
    const std::vector<double>& oy,
    const std::pair<double, double>& sweep_vec,
    const std::pair<double, double>& sweep_start_position);

std::pair<std::vector<double>, std::vector<double>> convert_global_coordinate(
    const std::vector<double>& x,
    const std::vector<double>& y,
    const std::pair<double, double>& sweep_vec,
    const std::pair<double, double>& sweep_start_position);

std::pair<std::vector<int>, std::optional<int>> search_free_grid_index_at_edge_y(
    GridMap& grid_map, bool from_upper = false);

std::tuple<GridMap, std::vector<int>, std::optional<int>> setup_grid_map(
    std::vector<double>& ox,
    std::vector<double>& oy,
    double resolution,
    int sweep_direction,
    int offset_grid = 10);

std::pair<std::vector<double>, std::vector<double>> sweepPathSearch(
    SweepSearcher& sweepSearcher, GridMap& gridMap);

std::pair<std::vector<double>, std::vector<double>> planning(
    const std::vector<double>& ox,
    const std::vector<double>& oy,
    double resolution,
    int movingDirection   = Direction::MovingDirection::RIGHT,
    int sweepingDirection = Direction::SweepDirection::UP);

#endif  // INCLUDE_INCLUDES_GRIDSWEEPCPP_H_
