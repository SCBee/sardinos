#ifndef HEADER_GRIDMAP
#define HEADER_GRIDMAP
#include <optional>
#include <vector>

struct GridMap
{
    /*
     * width: int (screen)
     * height: int (screen)
     * resolution: double (cell size -- change based on input values)
     *
     * centerX: double (center X coordinate for polygon based on input values)
     * centerY: double (center Y coordinate for polygon based on input values)
     *
     * left_lower_x: double (lower left corner x of polygon)
     * left_lower_y: double (lower left corner y of polygon)
     *
     * nData: int (number of cells)
     * data: vector<double> (value of cells, is it an obstruction? is it visited?)
     *
     */

    int width;
    int height;
    double resolution;

    double centerX;
    double centerY;

    double left_lower_x;
    double left_lower_y;

    int nData;
    std::vector<double> data;

    GridMap()
    {
    }
    GridMap(const int width,
            const int height,
            const double resolution,
            const double centerX,
            const double centerY,
            const double initVal = 0.0);

    std::optional<double> get_value_from_xy_index(const int ix, const int iy);

    std::pair<std::optional<int>, std::optional<int>> get_xy_index_from_xy_pos(
        const double x, const double y);

    bool set_value_from_xy_pos(const double x, const double y, const double val);

    bool set_value_from_xy_index(const std::optional<double> x,
                                 const std::optional<double> y,
                                 const double val);

    void set_value_from_polygon(std::vector<double>& x,
                                std::vector<double>& y,
                                const double val,
                                const bool& inside);

    int calc_grid_index_from_xy_index(const int ix, const int iy);

    std::pair<int, int> calc_xy_index_from_grid_index(const int grid_index);

    std::optional<int> calc_grid_index_from_xy_pos(const double x, const double y);

    std::pair<int, int> calc_grid_central_xy_position_from_grid_index(
        const int grid_index);

    std::pair<double, double> calc_grid_central_xy_position_from_xy_index(const int ix,
                                                                          const int iy);

    double calc_grid_central_xy_position_from_index(const int index,
                                                    const double lowerPos);

    std::optional<int> calc_xy_index_from_position(const double pos,
                                                   const double lowerPos,
                                                   const int maxIndex);

    bool check_occupied_from_xy_index(const int ix,
                                      const int iy,
                                      const double occupied_value);

    void expand_grid(const double occupied_value = 1.0);

    bool check_inside_polygon(const double ix,
                              const double iy,
                              const std::vector<double>& vx,
                              const std::vector<double>& vy);

    void print_grid_map_info();
};

#endif
