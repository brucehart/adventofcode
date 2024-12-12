#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// Structure to represent each reindeer
struct Reindeer {
    std::string name;     // Name of the reindeer
    int speed;            // Speed in km/s
    int fly_time;         // Time in seconds the reindeer can fly before resting
    int rest_time;        // Time in seconds the reindeer must rest

    // Function to calculate the distance traveled after a given total time
    long long distance_after(int total_time) const {
        int cycle_time = fly_time + rest_time; // Total time for one fly-rest cycle
        int complete_cycles = total_time / cycle_time; // Number of complete cycles
        int remaining_time = total_time % cycle_time;  // Remaining time after complete cycles

        // Time spent flying in the last (possibly incomplete) cycle
        int fly_time_in_last_cycle = std::min(remaining_time, fly_time);

        // Total distance = (complete cycles * fly_time * speed) + (fly_time_in_last_cycle * speed)
        return static_cast<long long>(complete_cycles) * fly_time * speed
             + static_cast<long long>(fly_time_in_last_cycle) * speed;
    }
};

// Function to parse a line of input and extract reindeer attributes
Reindeer parse_line(const std::string& line) {
    Reindeer r;
    std::istringstream iss(line);
    std::string tmp;

    // Extract the name
    iss >> r.name;

    // Skip "can"
    iss >> tmp;

    // Skip "fly"
    iss >> tmp;

    // Extract speed
    iss >> r.speed;

    // Skip "km/s"
    iss >> tmp;

    // Skip "for"
    iss >> tmp;

    // Extract fly_time
    iss >> r.fly_time;

    // Skip "seconds,"
    iss >> tmp;

    // Skip "but"
    iss >> tmp;

    // Skip "then"
    iss >> tmp;

    // Skip "must"
    iss >> tmp;

    // Skip "rest"
    iss >> tmp;

    // Skip "for"
    iss >> tmp;

    // Extract rest_time
    iss >> r.rest_time;

    // The remaining part "seconds." can be ignored

    return r;
}

int main() {
    std::vector<Reindeer> reindeers;
    std::string line;

    // Read each line from standard input and parse it into a Reindeer object
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            reindeers.push_back(parse_line(line));
        }
    }

    const int race_duration = 2503; // Total time of the race in seconds
    long long max_distance = 0;

    // Calculate the distance traveled by each reindeer and find the maximum distance
    for (const auto& reindeer : reindeers) {
        long long distance = reindeer.distance_after(race_duration);
        if (distance > max_distance) {
            max_distance = distance;
        }
    }

    // Output the maximum distance traveled by the winning reindeer
    std::cout << max_distance << std::endl;

    return 0;
}
