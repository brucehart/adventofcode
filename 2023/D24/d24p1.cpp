#include <iostream>
#include <vector>
#include <algorithm>

// Structure to represent a hailstone
struct Hailstone {
    long double px, py, pz;
    long double vx, vy, vz;
};

int main() {
    std::vector<Hailstone> hailstones;
    long double px, py, pz, vx, vy, vz;
    char comma, at;

    // Read hailstone data from input
    while (std::cin >> px >> comma >> py >> comma >> pz >> at >> vx >> comma >> vy >> comma >> vz) {
        hailstones.push_back({px, py, pz, vx, vy, vz});
    }

    long long intersection_count = 0;
    const long double min_coord = 200000000000000;
    const long double max_coord = 400000000000000;

    // Iterate through all pairs of hailstones
    for (size_t i = 0; i < hailstones.size(); ++i) {
        for (size_t j = i + 1; j < hailstones.size(); ++j) {
            const Hailstone& h1 = hailstones[i];
            const Hailstone& h2 = hailstones[j];

            // Calculate the intersection point of the paths in the X-Y plane
            long double det = h1.vx * h2.vy - h1.vy * h2.vx;
            if (det == 0) {
                continue; // Paths are parallel, no intersection
            }

            long double t1 = ((h2.px - h1.px) * h2.vy - (h2.py - h1.py) * h2.vx) / det;
            long double t2 = ((h2.px - h1.px) * h1.vy - (h2.py - h1.py) * h1.vx) / det;

            // Check if the intersection occurs in the future for both hailstones
            if (t1 < 0 || t2 < 0) {
                continue; 
            }

            long double intersect_x = h1.px + h1.vx * t1;
            long double intersect_y = h1.py + h1.vy * t1;

            // Check if the intersection point is within the test area
            if (intersect_x >= min_coord && intersect_x <= max_coord &&
                intersect_y >= min_coord && intersect_y <= max_coord) {
                intersection_count++;
            }
        }
    }

    std::cout << intersection_count << std::endl;

    return 0;
}
