#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

// Function to perform the Bron-Kerbosch algorithm with pivoting
void bron_kerbosch(
    const std::vector<std::vector<int>>& adj,
    std::vector<int>& R,
    std::vector<int>& P,
    std::vector<int>& X,
    std::vector<int>& max_clique
) {
    if (P.empty() && X.empty()) {
        if (R.size() > max_clique.size()) {
            max_clique = R;
        }
        return;
    }

    // Choose a pivot from P ? X
    int pivot = -1;
    int max_degree = -1;
    std::vector<int> union_PX;
    union_PX.reserve(P.size() + X.size());
    union_PX.insert(union_PX.end(), P.begin(), P.end());
    union_PX.insert(union_PX.end(), X.begin(), X.end());
    for (int u : union_PX) {
        if (static_cast<int>(adj[u].size()) > max_degree) {
            max_degree = adj[u].size();
            pivot = u;
        }
    }

    // P \ N(pivot)
    std::vector<int> candidates;
    std::unordered_set<int> neighbors;
    for (int neighbor : adj[pivot]) {
        neighbors.emplace(neighbor);
    }
    for (int v : P) {
        if (neighbors.find(v) == neighbors.end()) {
            candidates.push_back(v);
        }
    }

    for (int v : candidates) {
        R.push_back(v);

        // P n N(v)
        std::vector<int> P_intersection;
        std::set_intersection(
            P.begin(), P.end(),
            adj[v].begin(), adj[v].end(),
            std::back_inserter(P_intersection)
        );

        // X n N(v)
        std::vector<int> X_intersection;
        std::set_intersection(
            X.begin(), X.end(),
            adj[v].begin(), adj[v].end(),
            std::back_inserter(X_intersection)
        );

        bron_kerbosch(adj, R, P_intersection, X_intersection, max_clique);

        R.pop_back();

        // Move v from P to X
        P.erase(std::remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);
    }
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

    // Sort adjacency lists for efficient set operations
    for (auto& neighbors : adj) {
        std::sort(neighbors.begin(), neighbors.end());
    }

    // Initialize sets for Bron-Kerbosch
    std::vector<int> R; // Currently growing clique
    std::vector<int> P(n);
    std::vector<int> X;
    for (int i = 0; i < n; ++i) {
        P[i] = i;
    }

    std::vector<int> max_clique_indices;

    // Find the maximum clique
    bron_kerbosch(adj, R, P, X, max_clique_indices);

    // Extract node names from indices
    std::vector<std::string> max_clique;
    max_clique.reserve(max_clique_indices.size());
    for (int idx : max_clique_indices) {
        max_clique.push_back(nodes[idx]);
    }

    // Sort the names alphabetically
    std::sort(max_clique.begin(), max_clique.end());

    // Join the names with commas
    std::string password;
    for (size_t i = 0; i < max_clique.size(); ++i) {
        password += max_clique[i];
        if (i != max_clique.size() - 1) {
            password += ",";
        }
    }

    std::cout << password << std::endl;
    return 0;
}

