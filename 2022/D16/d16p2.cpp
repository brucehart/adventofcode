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
    // Example lines:
    // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    // Valve HH has flow rate=22; tunnel leads to valve GG

    // Extract the valve name
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
    // Read all input lines and parse valves
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

    // Identify valves with positive flow rates and assign each to a unique bitmask position
    std::vector<int> useful_valves;
    for (int i = 0; i < num_valves; ++i) {
        if (valves[i].flow_rate > 0) {
            useful_valves.push_back(i);
        }
    }

    int num_useful = static_cast<int>(useful_valves.size());

    // Initialize a vector to store the maximum pressure for each subset of valves
    // There are 2^num_useful possible subsets
    std::vector<long long> max_pressures(1 << num_useful, 0);

    // Starting valve "AA"
    int start = name_to_index["AA"];
    int total_time = 26; // Adjusted time after teaching the elephant

    // Recursive DFS to explore all possible sequences of valve openings
    // and update max_pressures for each subset
    std::function<void(int, int, int, long long)> dfs = [&](int current, int opened, int time_left, long long pressure) {
        // Update the maximum pressure for the current subset
        if (pressure > max_pressures[opened]) {
            max_pressures[opened] = pressure;
        }

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
                dfs(valve_index, opened | (1 << i), new_time, pressure + pressure_released);
            }
        }
    };

    // Start the DFS from the starting valve with no valves opened and total_time left
    dfs(start, 0, total_time, 0);

    // Now, iterate through all possible pairs of subsets where the subsets do not overlap
    // and find the maximum sum of their pressures
    long long max_total_pressure = 0;
    for (int s = 0; s < (1 << num_useful); ++s) {
        // The complement subset for the elephant
        int complement = ((1 << num_useful) - 1) ^ s;
        // Iterate through all subsets of the complement
        for (int t = complement; ; t = (t - 1) & complement) {
            // Update the maximum total pressure
            long long total = max_pressures[s] + max_pressures[t];
            if (total > max_total_pressure) {
                max_total_pressure = total;
            }
            if (t == 0) break; // All subsets of complement have been iterated
        }
    }

    // Output the result
    std::cout << max_total_pressure << std::endl;

    return 0;
}

