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

    // Fields for simulation
    long long distance = 0;       // Total distance traveled
    int points = 0;               // Points accumulated
    bool is_flying = true;        // Current state: flying or resting
    int time_in_state = 0;        // Time remaining in the current state

    // Initialize the reindeer's state
    void initialize() {
        is_flying = true;
        time_in_state = fly_time;
        distance = 0;
        points = 0;
    }

    // Update the reindeer's state for one second
    void update() {
        if (is_flying) {
            distance += speed;     // Increase distance by speed
            time_in_state -= 1;    // Decrease remaining fly time
            if (time_in_state == 0) {
                is_flying = false; // Switch to resting
                time_in_state = rest_time;
            }
        } else {
            time_in_state -= 1;    // Decrease remaining rest time
            if (time_in_state == 0) {
                is_flying = true;  // Switch to flying
                time_in_state = fly_time;
            }
        }
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

    // Initialize each reindeer's state before the race starts
    for (auto& reindeer : reindeers) {
        reindeer.initialize();
    }

    // Simulate the race second by second
    for (int second = 1; second <= race_duration; ++second) {
        // Update each reindeer's state and distance
        for (auto& reindeer : reindeers) {
            reindeer.update();
        }

        // Determine the maximum distance achieved so far
        long long current_max_distance = std::max_element(
            reindeers.begin(),
            reindeers.end(),
            [](const Reindeer& a, const Reindeer& b) {
                return a.distance < b.distance;
            }
        )->distance;

        // Award one point to each reindeer that is currently in the lead
        for (auto& reindeer : reindeers) {
            if (reindeer.distance == current_max_distance) {
                reindeer.points += 1;
            }
        }
    }

    // Find the maximum points accumulated by any reindeer
    int max_points = std::max_element(
        reindeers.begin(),
        reindeers.end(),
        [](const Reindeer& a, const Reindeer& b) {
            return a.points < b.points;
        }
    )->points;

    // Output the maximum points
    std::cout << max_points << std::endl;

    return 0;
}

