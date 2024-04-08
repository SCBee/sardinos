#include <cmath>

#include "Utils.h"

namespace utils
{
    std::array<std::array<double, 2>, 2> rot_mat_2d(double radians)
    {
        // Convert angle from degrees to radians for std::cos and std::sin
        /* radians *=  M_PI / 180.0; */

        std::array<std::array<double, 2>, 2> rotationMatrix = {
            {{std::cos(radians), -std::sin(radians)},
             {std::sin(radians), std::cos(radians)}}};

        return rotationMatrix;
    }
}  // namespace utils
