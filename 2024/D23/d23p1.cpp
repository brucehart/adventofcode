#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

// Function to check if any of the three node names starts with 't'
bool has_t(const std::vector<std::string>& nodes, int i, int j, int k) {
    return !nodes[i].empty() && nodes[i][0] == 't' ||
           !nodes[j].empty() && nodes[j][0] == 't' ||
           !nodes[k].empty() && nodes[k][0] == 't';
}

int main() {
    std::vector<std::pair<std::string, std::string>> connections;
    std::string line;
    std::unordered_set<std::string> node_set;

    // Read all connections and collect unique node names
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        size_t dash = line.find('-');
        if (dash == std::string::npos) continue; // Invalid line
        std::string a = line.substr(0, dash);
        std::string b = line.substr(dash + 1);
        connections.emplace_back(a, b);
        node_set.emplace(a);
        node_set.emplace(b);
    }

    // Assign each node a unique index based on sorted order
    std::vector<std::string> nodes(node_set.begin(), node_set.end());
    std::sort(nodes.begin(), nodes.end());
    std::unordered_map<std::string, int> node_to_index;
    for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
        node_to_index[nodes[i]] = i;
    }

    int n = nodes.size();
    std::vector<std::vector<int>> adj(n, std::vector<int>());

    // Build adjacency lists
    for (const auto& [a, b] : connections) {
        int u = node_to_index[a];
        int v = node_to_index[b];
        if (u != v) { // Avoid self-loops
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    // Sort adjacency lists for efficient intersection
    for (auto& neighbors : adj) {
        std::sort(neighbors.begin(), neighbors.end());
    }

    int triangle_count = 0;

    // Enumerate all triangles
    for (int u = 0; u < n; ++u) {
        for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
            int v = *it;
            if (v > u) { // Ensure u < v to avoid duplicates
                // Find common neighbors of u and v
                std::vector<int> intersection;
                std::set_intersection(
                    adj[u].begin(), adj[u].end(),
                    adj[v].begin(), adj[v].end(),
                    std::back_inserter(intersection)
                );
                for (int w : intersection) {
                    if (w > v) { // Ensure v < w to avoid duplicates
                        if (has_t(nodes, u, v, w)) {
                            ++triangle_count;
                        }
                    }
                }
            }
        }
    }

    std::cout << triangle_count << std::endl;
    return 0;
}

