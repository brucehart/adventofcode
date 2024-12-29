#include <bits/stdc++.h>

// Structure to represent each valve with its flow rate and connected valves
struct Valve {
    std::string name;
    int flow_rate;
    std::vector<std::string> connections;
};

// Function to parse a single line of input and return a Valve object
Valve parse_line(const std::string& line) {
    Valve valve;
    // Example line:
    // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    // Valve HH has flow rate=22; tunnel leads to valve GG

    // Find the position of "Valve " and extract the valve name
    size_t pos = line.find("Valve ");
    pos += 6; // Move past "Valve "
    size_t end = line.find(" has flow rate=", pos);
    valve.name = line.substr(pos, end - pos);

    // Extract the flow rate
    pos = end + 15; // Move past " has flow rate="
    end = line.find(';', pos);
    valve.flow_rate = std::stoi(line.substr(pos, end - pos));

    // Extract the connections
    pos = line.find("valves ", end);
    if (pos == std::string::npos) {
        pos = line.find("valve ", end);
        pos += 6; // Move past "valve "
    } else {
        pos += 7; // Move past "valves "
    }

    std::string connections_str = line.substr(pos);
    // Split by ", " to get individual valve names
    std::stringstream ss(connections_str);
    std::string connection;
    while (std::getline(ss, connection, ',')) {
        // Remove any leading spaces
        connection.erase(0, connection.find_first_not_of(' '));
        valve.connections.push_back(connection);
    }

    return valve;
}

int main() {
    // Read all input lines
    std::vector<Valve> valves;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            valves.push_back(parse_line(line));
        }
    }

    // Map valve names to indices for easy reference
    std::unordered_map<std::string, int> name_to_index;
    for (size_t i = 0; i < valves.size(); ++i) {
        name_to_index[valves[i].name] = static_cast<int>(i);
    }

    int num_valves = static_cast<int>(valves.size());

    // Precompute shortest distances between all pairs of valves using BFS
    std::vector<std::vector<int>> distances(num_valves, std::vector<int>(num_valves, num_valves));
    for (int i = 0; i < num_valves; ++i) {
        distances[i][i] = 0;
        std::queue<int> q;
        q.push(i);
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            for (const auto& neighbor_name : valves[current].connections) {
                int neighbor = name_to_index[neighbor_name];
                if (distances[i][neighbor] == num_valves) { // Not visited
                    distances[i][neighbor] = distances[i][current] + 1;
                    q.push(neighbor);
                }
            }
        }
    }

    // Identify valves with positive flow rates
    std::vector<int> useful_valves;
    for (int i = 0; i < num_valves; ++i) {
        if (valves[i].flow_rate > 0) {
            useful_valves.push_back(i);
        }
    }

    int num_useful = static_cast<int>(useful_valves.size());

    // Memoization cache: key = (current valve, bitmask of opened valves, time left)
    // Value = maximum pressure achievable from this state
    // Using a 3D vector for memoization
    // Initialize with -1 indicating uncomputed states
    std::vector<std::vector<std::vector<long long>>> memo(
        num_valves,
        std::vector<std::vector<long long>>(1 << num_useful, std::vector<long long>(31, -1))
    );

    // Recursive DFS function to explore all possible sequences of valve openings
    std::function<long long(int, int, int)> dfs = [&](int current, int opened, int time_left) -> long long {
        // If time is up, no more pressure can be released
        if (time_left <= 0) {
            return 0;
        }

        // Check if this state has been computed before
        if (memo[current][opened][time_left] != -1) {
            return memo[current][opened][time_left];
        }

        long long max_pressure = 0;

        // Iterate over all useful valves to consider opening them
        for (int i = 0; i < num_useful; ++i) {
            // Check if this valve is already opened
            if (opened & (1 << i)) {
                continue;
            }

            int valve_index = useful_valves[i];
            // Calculate the time to move to this valve and open it
            int time_to_open = distances[current][valve_index] + 1;

            if (time_left >= time_to_open) {
                // Calculate the remaining time after opening the valve
                int new_time = time_left - time_to_open;
                // Calculate the pressure released by this valve
                long long pressure_released = static_cast<long long>(valves[valve_index].flow_rate) * new_time;
                // Recurse to open the next valves
                long long total_pressure = pressure_released + dfs(valve_index, opened | (1 << i), new_time);
                // Update the maximum pressure
                max_pressure = std::max(max_pressure, total_pressure);
            }
        }

        // Also consider the option of not opening any more valves
        // (i.e., ending the sequence here)
        // This is implicitly handled by not adding any more pressure

        // Memoize the computed result
        memo[current][opened][time_left] = max_pressure;
        return max_pressure;
    };

    // Starting at valve "AA"
    int start = name_to_index["AA"];
    int total_time = 30;

    long long result = dfs(start, 0, total_time);

    // Output the result
    std::cout << result << std::endl;

    return 0;
}

