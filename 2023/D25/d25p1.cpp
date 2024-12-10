#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <random>
#include <queue>

// Function to perform Depth First Search and count reachable nodes
int dfs(const std::string& start, const std::map<std::string, std::set<std::string>>& adj,
        std::set<std::pair<std::string, std::string>>& cut_edges, std::set<std::string>& visited) {
    visited.insert(start);
    int count = 1;
    for (const std::string& neighbor : adj.at(start)) {
        if (visited.find(neighbor) == visited.end() &&
            cut_edges.find({start, neighbor}) == cut_edges.end() &&
            cut_edges.find({neighbor, start}) == cut_edges.end()) {
            count += dfs(neighbor, adj, cut_edges, visited);
        }
    }
    return count;
}

int main() {
    std::map<std::string, std::set<std::string>> adj;
    std::string line;

    // Read input and build adjacency list
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string u, v;
        iss >> u;
        u.pop_back(); // Remove colon
        while (iss >> v) {
            adj[u].insert(v);
            adj[v].insert(u);
        }
    }

    // Sample paths between random pairs of nodes
    std::map<std::pair<std::string, std::string>, int> edge_counts;
    std::vector<std::string> nodes;
    for (const auto& pair : adj) {
        nodes.push_back(pair.first);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, nodes.size() - 1);

    for (int i = 0; i < 1000; ++i) {
        int start_idx = distrib(gen);
        int end_idx = distrib(gen);
        if (start_idx == end_idx) continue;

        std::string start = nodes[start_idx];
        std::string end = nodes[end_idx];

        std::queue<std::string> q;
        q.push(start);
        std::map<std::string, std::string> parent;
        std::set<std::string> visited;
        visited.insert(start);

        while (!q.empty()) {
            std::string u = q.front();
            q.pop();

            if (u == end) break;

            for (const std::string& v : adj.at(u)) {
                if (visited.find(v) == visited.end()) {
                    visited.insert(v);
                    q.push(v);
                    parent[v] = u;
                }
            }
        }

        // Trace back the path and increment edge counts
        std::string current = end;
        while (current != start) {
            std::string p = parent[current];
            if (p < current)
              edge_counts[{p, current}]++;
            else
              edge_counts[{current,p}]++;
            current = p;
        }
    }

    // Sort edges by frequency
    std::vector<std::pair<std::pair<std::string, std::string>, int>> sorted_edges(edge_counts.begin(), edge_counts.end());
    std::sort(sorted_edges.begin(), sorted_edges.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    // Cut the top 3 edges
    std::set<std::pair<std::string, std::string>> cut_edges;
    for (int i = 0; i < 3; ++i) {
        cut_edges.insert(sorted_edges[i].first);
    }

    // Calculate the size of the first group
    std::set<std::string> visited;
    int group1_size = dfs(nodes[0], adj, cut_edges, visited);

    // Calculate the size of the second group
    int group2_size = nodes.size() - group1_size;

    // Output the result
    std::cout << group1_size * group2_size << std::endl;

    return 0;
}