#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <optional>

#include "GridMap.h"

GridMap::GridMap(const int width,
                 const int height,
                 const double resolution,
                 const double centerX,
                 const double centerY,
                 const double initVal)
{
    this->width      = width;
    this->height     = height;
    this->resolution = resolution;
    this->centerX    = centerX;
    this->centerY    = centerY;

    this->left_lower_x = centerX - (static_cast<double>(width) / 2.0) * resolution;
    this->left_lower_y = centerY - (static_cast<double>(height) / 2.0) * resolution;

    nData = width * height;
    for (int i = 0; i < nData; ++i)
    {
        data.push_back(0.0);
    }
}

std::optional<double> GridMap::get_value_from_xy_index(const int ix, const int iy)
{
    int grid_index = calc_grid_index_from_xy_index(ix, iy);

    if (grid_index >= 0 and grid_index < this->nData)
    {
        return this->data[grid_index];
    }
    else
    {
        return std::nullopt;
    }
}

std::pair<std::optional<int>, std::optional<int>> GridMap::get_xy_index_from_xy_pos(
    const double x, const double y)
{
    std::optional<int> ix =
        calc_xy_index_from_position(x, this->left_lower_x, this->width);
    std::optional<int> iy =
        calc_xy_index_from_position(y, this->left_lower_y, this->height);

    if (ix.has_value() and iy.has_value())
    {
        return {ix.value(), iy.value()};
    }
    else
    {
        return {std::nullopt, std::nullopt};
    }
}

bool GridMap::set_value_from_xy_pos(const double x, const double y, const double val)
{
    auto p = get_xy_index_from_xy_pos(x, y);

    if (!p.first.has_value() or !p.second.has_value())
    {
        return false;
    }

    return set_value_from_xy_index(p.first.value(), p.second.value(), val);
}

bool GridMap::set_value_from_xy_index(const std::optional<double> x,
                                      const std::optional<double> y,
                                      const double val)
{
    if (!x.has_value() or !y.has_value())
    {
        return false;
    }

    int grid_index = y.value() * this->width + x.value();

    if (grid_index >= 0 and grid_index < this->nData)
    {
        this->data[grid_index] = val;
        return true;
    }

    return false;
}

void GridMap::set_value_from_polygon(std::vector<double>& x,
                                     std::vector<double>& y,
                                     const double val,
                                     const bool& inside)
{
    if ((x[0] != x[x.size() - 1]) or (y[0] != y[y.size() - 1]))
    {
        x.push_back(x[0]);
        y.push_back(y[0]);
    }

    for (int i = 0; i < this->width; ++i)
    {
        for (int j = 0; j < this->height; ++j)
        {
            auto [x_pos, y_pos] = calc_grid_central_xy_position_from_xy_index(i, j);

            bool flag = check_inside_polygon(x_pos, y_pos, x, y);

            if (flag == inside)
            {
                set_value_from_xy_index(i, j, val);
            }
        }
    }
}

int GridMap::calc_grid_index_from_xy_index(const int ix, const int iy)
{
    return iy * this->width + ix;
}

std::pair<int, int> GridMap::calc_xy_index_from_grid_index(const int grid_index)
{
    int ix = grid_index / this->width;
    int iy = std::fmod(grid_index, this->width);

    return {ix, iy};
}

std::optional<int> GridMap::calc_grid_index_from_xy_pos(const double x, const double y)
{
    std::optional<int> ix =
        calc_xy_index_from_position(x, this->left_lower_x, this->width);
    std::optional<int> iy =
        calc_xy_index_from_position(y, this->left_lower_y, this->height);

    if (ix.has_value() and iy.has_value())
    {
        return calc_grid_index_from_xy_index(ix.value(), iy.value());
    }
    else
    {
        return std::nullopt;
    }
}

std::pair<int, int> GridMap::calc_grid_central_xy_position_from_grid_index(
    const int grid_index)
{
    return calc_xy_index_from_grid_index(grid_index);
}

std::pair<double, double> GridMap::calc_grid_central_xy_position_from_xy_index(
    const int ix, const int iy)
{
    double x = calc_grid_central_xy_position_from_index(ix, this->left_lower_x);
    double y = calc_grid_central_xy_position_from_index(iy, this->left_lower_y);

    return {x, y};
}

double GridMap::calc_grid_central_xy_position_from_index(const int index,
                                                         const double lowerPos)
{
    return lowerPos + index * this->resolution + this->resolution / 2.0;
}

std::optional<int> GridMap::calc_xy_index_from_position(const double pos,
                                                        const double lowerPos,
                                                        const int maxIndex)
{
    int index = std::floor((pos - lowerPos) / this->resolution);
    if (index >= 0 and index <= maxIndex)
    {
        return index;
    }
    else
    {
        return std::nullopt;
    }
}

bool GridMap::check_occupied_from_xy_index(const int ix,
                                           const int iy,
                                           const double occupied_value)
{
    auto val = get_value_from_xy_index(ix, iy);

    if (!val.has_value())
    {
        return true;
    }
    else if (val.value() >= occupied_value)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GridMap::expand_grid(const double occupied_value)
{
    std::vector<int> ix;
    std::vector<int> iy;
    std::vector<double> vals;

    for (int i = 0; i < this->width; ++i)
    {
        for (int j = 0; j < this->height; ++j)
        {
            if (check_occupied_from_xy_index(i, j, occupied_value))
            {
                std::optional<double> val = get_value_from_xy_index(i, j);
                if (val.has_value())
                {
                    ix.push_back(i);
                    iy.push_back(j);
                    vals.push_back(val.value());
                }
            }
        }
    }

    auto min_size = std::min({ix.size(), iy.size(), vals.size()});

    for (int i = 0; i < min_size; ++i)
    {
        set_value_from_xy_index(ix[i] + 1, iy[i], vals[i]);
        set_value_from_xy_index(ix[i], iy[i] + 1, vals[i]);
        set_value_from_xy_index(ix[i] + 1, iy[i] + 1, vals[i]);

        set_value_from_xy_index(ix[i] - 1, iy[i], vals[i]);
        set_value_from_xy_index(ix[i], iy[i] - 1, vals[i]);
        set_value_from_xy_index(ix[i] - 1, iy[i] - 1, vals[i]);
    }
}

bool GridMap::check_inside_polygon(const double ix,
                                   const double iy,
                                   const std::vector<double>& vx,
                                   const std::vector<double>& vy)
{
    int n = std::min(vx.size(), vy.size()) - 1;

    bool inside = false;
    int minX    = INT_MAX;
    int maxX    = -INT_MAX;

    for (int i = 0; i < n; ++i)
    {
        int j = (i + 1) % (n + 1);

        if (vx[i] >= vx[j])
        {
            minX = vx[j];
            maxX = vx[i];
        }
        else
        {
            minX = vx[i];
            maxX = vx[j];
        }
        if (!(minX <= ix and ix < maxX))
        {
            continue;
        }

        double tmp = (vy[j] - vy[i]) / (vx[j] - vx[i]);
        if ((vy[i] + tmp * (ix - vx[i]) - iy) > 0.0)
        {
            inside = !inside;
        }
    }
    return inside;
}

void GridMap::print_grid_map_info()
{
    std::cout << "width: " << this->width << "\n";
    std::cout << "height: " << this->height << "\n";
    std::cout << "resolution: " << this->resolution << "\n";
    std::cout << "centerX: " << this->centerX << "\n";
    std::cout << "centerY: " << this->centerY << "\n";
    std::cout << "left_lower_x: " << this->left_lower_x << "\n";
    std::cout << "left_lower_y: " << this->left_lower_y << "\n";
    std::cout << "n_data: " << this->nData << "\n";
}
