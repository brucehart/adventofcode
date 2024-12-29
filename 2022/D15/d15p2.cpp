#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <cmath>
#include <unordered_set>

// Structure to hold sensor and beacon coordinates
struct SensorBeacon {
    long long sensor_x;
    long long sensor_y;
    long long beacon_x;
    long long beacon_y;
    long long distance; // Manhattan distance between sensor and beacon
};

// Function to parse a single input line and extract sensor and beacon coordinates
bool parse_line(const std::string& line, SensorBeacon& sb) {
    // Expected format:
    // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
    // We'll parse the line manually to extract the numbers

    sb.distance = 0; // Initialize distance

    size_t sx = line.find("x=") + 2;
    size_t sy = line.find("y=", sx) + 2;
    size_t bx = line.find("x=", sy) + 2;
    size_t by = line.find("y=", bx) + 2;

    if (sx == std::string::npos || sy == std::string::npos ||
        bx == std::string::npos || by == std::string::npos) {
        return false;
    }

    // Extract sensor_x
    size_t comma = line.find(",", sx);
    if (comma == std::string::npos) return false;
    sb.sensor_x = std::stoll(line.substr(sx, comma - sx));

    // Extract sensor_y
    size_t colon = line.find(":", sy);
    if (colon == std::string::npos) return false;
    sb.sensor_y = std::stoll(line.substr(sy, colon - sy));

    // Extract beacon_x
    comma = line.find(",", bx);
    if (comma == std::string::npos) return false;
    sb.beacon_x = std::stoll(line.substr(bx, comma - bx));

    // Extract beacon_y
    // y may be followed by end of line
    size_t end = line.find_first_of(" \n\r", by);
    if (end == std::string::npos) end = line.length();
    sb.beacon_y = std::stoll(line.substr(by, end - by));

    // Calculate Manhattan distance
    sb.distance = std::abs(sb.sensor_x - sb.beacon_x) + std::abs(sb.sensor_y - sb.beacon_y);

    return true;
}

// Function to check if a given point is within any sensor's coverage
bool is_covered(long long x, long long y, const std::vector<SensorBeacon>& sensors) {
    for (const auto& sb : sensors) {
        long long dist = std::abs(sb.sensor_x - x) + std::abs(sb.sensor_y - y);
        if (dist <= sb.distance) {
            return true;
        }
    }
    return false;
}

int main() {
    std::vector<SensorBeacon> sensor_beacons;
    std::string line;

    // Read input from stdin
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue; // Skip empty lines
        SensorBeacon sb;
        if (parse_line(line, sb)) {
            sensor_beacons.push_back(sb);
        }
    }

    // Define the search space boundaries
    const long long MIN_COORD = 0;
    const long long MAX_COORD = 4000000;

    // Iterate through each sensor to generate perimeter points (distance + 1)
    for (const auto& sb : sensor_beacons) {
        long long d = sb.distance + 1;

        // Generate points along the perimeter of the diamond (Manhattan distance)
        for (long long dx = 0; dx <= d; ++dx) {
            long long dy = d - dx;

            // Possible positions (x, y)
            std::vector<std::pair<long long, long long>> candidates = {
                {sb.sensor_x + dx, sb.sensor_y + dy},
                {sb.sensor_x + dx, sb.sensor_y - dy},
                {sb.sensor_x - dx, sb.sensor_y + dy},
                {sb.sensor_x - dx, sb.sensor_y - dy}
            };

            for (const auto& [x, y] : candidates) {
                // Check if the point is within the search boundaries
                if (x < MIN_COORD || x > MAX_COORD || y < MIN_COORD || y > MAX_COORD) {
                    continue;
                }

                // Check if this point is not covered by any sensor
                if (!is_covered(x, y, sensor_beacons)) {
                    // Found the distress beacon
                    long long tuning_frequency = x * 4000000 + y;
                    std::cout << tuning_frequency << std::endl;
                    return 0;
                }
            }
        }
    }

    // If no distress beacon is found
    std::cout << "No distress beacon found." << std::endl;
    return 0;
}

