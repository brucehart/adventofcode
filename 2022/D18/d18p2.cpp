#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>
#include <queue>
#include <array>
#include <algorithm>
#include <limits>

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
    // Variables to track the minimum and maximum coordinates
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();
    int minZ = std::numeric_limits<int>::max();
    int maxZ = std::numeric_limits<int>::min();

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

        // Update the bounding box
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
        minZ = std::min(minZ, p.z);
        maxZ = std::max(maxZ, p.z);
    }

    // Define the directions representing the six adjacent neighbors in 3D space
    const std::array<std::array<int, 3>, 6> directions = {{
        {1, 0, 0},  // +x
        {-1, 0, 0}, // -x
        {0, 1, 0},  // +y
        {0, -1, 0}, // -y
        {0, 0, 1},  // +z
        {0, 0, -1}  // -z
    }};

    // Extend the bounding box by 1 in all directions to ensure flood fill starts outside
    minX -= 1;
    minY -= 1;
    minZ -= 1;
    maxX += 1;
    maxY += 1;
    maxZ += 1;

    // Define a queue for BFS and a set to keep track of visited positions
    std::queue<Point> q;
    std::unordered_set<Point, PointHash> visited;

    // Start BFS from the position outside the bounding box
    Point start = {minX, minY, minZ};
    q.push(start);
    visited.insert(start);

    // Perform BFS to find all external air positions
    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        // Explore all six adjacent neighbors
        for (const auto& dir : directions) {
            Point neighbor = {current.x + dir[0], current.y + dir[1], current.z + dir[2]};

            // Check if neighbor is within the extended bounding box
            if (neighbor.x < minX || neighbor.x > maxX ||
                neighbor.y < minY || neighbor.y > maxY ||
                neighbor.z < minZ || neighbor.z > maxZ) {
                continue; // Skip positions outside the bounding box
            }

            // If neighbor is part of the lava droplet or already visited, skip
            if (cubes.find(neighbor) != cubes.end() || visited.find(neighbor) != visited.end()) {
                continue;
            }

            // Mark neighbor as visited and add to the queue
            visited.insert(neighbor);
            q.push(neighbor);
        }
    }

    long long exterior_surface_area = 0;

    // Iterate through each cube to calculate its exterior exposed sides
    for (const auto& cube : cubes) {
        // Check all six possible neighboring positions
        for (const auto& dir : directions) {
            Point neighbor = {cube.x + dir[0], cube.y + dir[1], cube.z + dir[2]};
            // If the neighbor is an external air position, the side is exterior
            if (visited.find(neighbor) != visited.end()) {
                ++exterior_surface_area;
            }
        }
    }

    // Output the exterior surface area followed by a newline
    std::cout << exterior_surface_area << std::endl;

    return 0;
}

