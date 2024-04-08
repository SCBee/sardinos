#ifndef INCLUDE_INCLUDES_UTILS_H_
#define INCLUDE_INCLUDES_UTILS_H_
#include <array>

namespace utils
{
    std::array<std::array<double, 2>, 2> rot_mat_2d(double radians);
}  // namespace utils

#endif  // INCLUDE_INCLUDES_UTILS_H_
