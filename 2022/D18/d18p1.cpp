#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

// Structure to represent a 3D point (cube position)
struct Point {
    int x;
    int y;
    int z;

    // Equality operator to compare two points
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Custom hash function for Point to be used in unordered_set
struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        // Use prime numbers to combine the hash of each coordinate
        // This reduces the likelihood of collisions
        std::size_t hx = std::hash<int>()(p.x);
        std::size_t hy = std::hash<int>()(p.y);
        std::size_t hz = std::hash<int>()(p.z);
        return hx * 73856093 ^ hy * 19349663 ^ hz * 83492791;
    }
};

int main() {
    // Define an unordered_set to store all unique cube positions
    std::unordered_set<Point, PointHash> cubes;

    std::string line;
    // Read input line by line from standard input
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue; // Skip empty lines if any
        }

        std::stringstream ss(line);
        std::string token;
        Point p;
        // Parse the x, y, z coordinates separated by commas
        if (std::getline(ss, token, ',')) {
            p.x = std::stoi(token);
        }
        if (std::getline(ss, token, ',')) {
            p.y = std::stoi(token);
        }
        if (std::getline(ss, token, ',')) {
            p.z = std::stoi(token);
        }

        // Insert the parsed point into the set
        cubes.insert(p);
    }

    // Directions representing the six adjacent neighbors in 3D space
    const int directions[6][3] = {
        {1, 0, 0},  // +x
        {-1, 0, 0}, // -x
        {0, 1, 0},  // +y
        {0, -1, 0}, // -y
        {0, 0, 1},  // +z
        {0, 0, -1}  // -z
    };

    long long total_surface_area = 0;

    // Iterate through each cube to calculate its exposed sides
    for (const auto& cube : cubes) {
        // Check all six possible neighboring positions
        for (const auto& dir : directions) {
            Point neighbor = {cube.x + dir[0], cube.y + dir[1], cube.z + dir[2]};
            // If the neighbor cube does not exist, the side is exposed
            if (cubes.find(neighbor) == cubes.end()) {
                ++total_surface_area;
            }
        }
    }

    // Output the total surface area followed by a newline
    std::cout << total_surface_area << std::endl;

    return 0;
}

