#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <cmath>

// Structure to hold sensor and beacon coordinates
struct SensorBeacon {
    long long sensor_x;
    long long sensor_y;
    long long beacon_x;
    long long beacon_y;
};

// Function to parse a single input line and extract sensor and beacon coordinates
bool parse_line(const std::string& line, SensorBeacon& sb) {
    // Expected format:
    // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
    std::istringstream iss(line);
    std::string token;
    
    // Parse "Sensor", "at", "x=2,", "y=18:", "closest", "beacon", "is", "at", "x=-2,", "y=15"
    if (!(iss >> token) || token != "Sensor") return false;
    if (!(iss >> token) || token != "at") return false;
    
    // Parse sensor_x
    if (!std::getline(iss, token, '=')) return false; // Skip "x="
    if (!(iss >> sb.sensor_x)) return false;
    if (!(iss >> token)) return false; // Skip ","
    
    // Parse sensor_y
    if (!std::getline(iss, token, '=')) return false; // Skip "y="
    if (!(iss >> sb.sensor_y)) return false;
    if (!(iss >> token)) return false; // Skip ":"
    
    // Parse "closest", "beacon", "is", "at", "x=-2,", "y=15"
    if (!(iss >> token) || token != "closest") return false;
    if (!(iss >> token) || token != "beacon") return false;
    if (!(iss >> token) || token != "is") return false;
    if (!(iss >> token) || token != "at") return false;
    
    // Parse beacon_x
    if (!std::getline(iss, token, '=')) return false; // Skip "x="
    if (!(iss >> sb.beacon_x)) return false;
    if (!(iss >> token)) return false; // Skip ","
    
    // Parse beacon_y
    if (!std::getline(iss, token, '=')) return false; // Skip "y="
    if (!(iss >> sb.beacon_y)) return false;
    
    return true;
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
    
    // The target row to analyze
    const long long target_y = 2000000;
    
    // Vector to store intervals on the target row where beacons cannot exist
    std::vector<std::pair<long long, long long>> intervals;
    
    // Set to store unique beacon x positions on the target row
    std::set<long long> beacons_on_target;
    
    for (const auto& sb : sensor_beacons) {
        // Calculate Manhattan distance between sensor and its closest beacon
        long long distance = std::abs(sb.sensor_x - sb.beacon_x) + std::abs(sb.sensor_y - sb.beacon_y);
        
        // Calculate vertical distance from sensor to target row
        long long vertical_distance = std::abs(sb.sensor_y - target_y);
        
        // If the sensor's coverage reaches the target row
        if (vertical_distance <= distance) {
            // Calculate the horizontal range on the target row
            long long remaining_distance = distance - vertical_distance;
            long long start_x = sb.sensor_x - remaining_distance;
            long long end_x = sb.sensor_x + remaining_distance;
            intervals.emplace_back(start_x, end_x);
        }
        
        // If the beacon is on the target row, record its x position
        if (sb.beacon_y == target_y) {
            beacons_on_target.insert(sb.beacon_x);
        }
    }
    
    if (intervals.empty()) {
        // No coverage on the target row
        std::cout << "0" << std::endl;
        return 0;
    }
    
    // Sort intervals based on starting x
    std::sort(intervals.begin(), intervals.end(), 
        [](const std::pair<long long, long long>& a, const std::pair<long long, long long>& b) -> bool {
            if (a.first != b.first) return a.first < b.first;
            return a.second < b.second;
        }
    );
    
    // Merge overlapping or contiguous intervals
    std::vector<std::pair<long long, long long>> merged;
    merged.emplace_back(intervals[0].first, intervals[0].second);
    
    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i].first <= merged.back().second + 1) {
            // Overlapping or contiguous intervals, merge them
            merged.back().second = std::max(merged.back().second, intervals[i].second);
        } else {
            // Non-overlapping interval, add to merged list
            merged.emplace_back(intervals[i].first, intervals[i].second);
        }
    }
    
    // Calculate the total number of positions covered by merged intervals
    long long total_covered = 0;
    for (const auto& interval : merged) {
        total_covered += (interval.second - interval.first + 1);
    }
    
    // Subtract the number of beacons that are actually on the target row within the covered intervals
    for (const auto& bx : beacons_on_target) {
        for (const auto& interval : merged) {
            if (bx >= interval.first && bx <= interval.second) {
                total_covered -= 1;
                break; // Avoid double-counting if a beacon is covered by multiple intervals
            }
        }
    }
    
    // Output the result
    std::cout << total_covered << std::endl;
    
    return 0;
}

