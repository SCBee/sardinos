#include <iostream>
#include <vector>  // For using the vector

#include "GridSweepCPP.h"

int main()
{
    std::vector<double> ox {0.0, 20.0, 50.0, 100.0, 130.0, 40.0, 0.0};
    std::vector<double> oy {0.0, -20.0, 0.0, 30.0, 60.0, 80.0, 0.0};
    double resolution = 5.0;

    auto [px, py] = planning(ox, oy, resolution);

    std::cout << "Dimensions (x, y): " << px.size() << " " << py.size() << "\n";

    return 0;
}
